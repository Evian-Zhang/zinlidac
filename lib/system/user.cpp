#include "system.h"
#include "util.h"

#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace libzinlidac {
// throws a `SpecialError` if name of the user logged in cannot be determined
LoginUserInfo get_associated_user() {
    // From `man getlogin`:
    //
    // getlogin()  returns  a  pointer  to a string containing the name of the
    // user logged in on the controlling terminal of the process,  or  a  null
    // pointer if this information cannot be determined.
    char *username = getlogin();
    if (username != NULL) {
        return (LoginUserInfo){
            .name = std::string(username),
            .uid = getuid()
        };
    } else {
        throw system::SpecialError("Cannot determine logged in user");
    }
}

// throws a `FileReadError` if cannot open /etc/passwd
// throws a `SpecialError` if cannot parse uid or groupid to integer
std::vector<UserInfo> get_users() {
    std::vector<UserInfo> user_infos;
    auto passwd_file = std::ifstream("/etc/passwd");
    if (!passwd_file.is_open()) {
        throw system::FileReadError("/etc/passwd");
    }
    std::string passwd_line;
    auto regex = std::regex("(.*?):.*?:(.*?):(.*?):(.*?):(.*?):(.*)");
    std::smatch sm;
    while (std::getline(passwd_file, passwd_line)) {
        if (std::regex_match(passwd_line, sm, regex)) {
            if (sm.size() == 7) {
                unsigned int uid, groupid;
                try {
                    uid = std::stoi(sm[2]);
                    groupid = std::stoi(sm[3]);
                } catch (...) {
                    throw system::SpecialError(std::string("Cannot parse id to integer"));
                }
                user_infos.push_back((UserInfo){
                    .name = sm[1],
                    .uid = uid,
                    .groupid = groupid,
                    .full_name = sm[4],
                    .home_directory = sm[5],
                    .login_shell = sm[6]
                });
            }
        }
    }
    return user_infos;
}

// see https://stackoverflow.com/a/5607650/10005095
struct UserListTokens : std::ctype<char> {
    UserListTokens() : std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const *get_table() {
        typedef std::ctype<char> cctype;
        static const cctype::mask *const_rc= cctype::classic_table();

        static cctype::mask rc[cctype::table_size];
        std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

        rc[','] = std::ctype_base::space; 
        return &rc[0];
    }
};

// throws a `FileReadError` if cannot open /etc/group
// throws a `SpecialError` if cannot parse gid to integer
std::vector<GroupInfo> get_groups() {
    std::vector<GroupInfo> group_infos;
    auto group_file = std::ifstream("/etc/group");
    if (!group_file.is_open()) {
        throw system::FileReadError("/etc/group");
    }
    std::string group_line;
    auto regex = std::regex("(.*?):.*?:(.*?):(.*)");
    std::smatch sm;
    while (std::getline(group_file, group_line)) {
        if (std::regex_match(group_line, sm, regex)) {
            if (sm.size() == 4) {
                unsigned int gid;
                try {
                    gid = std::stoi(sm[2]);
                } catch (...) {
                    throw system::SpecialError(std::string("Cannot parse gid to integer"));
                }
                std::string user_list = sm[3];
                // see https://stackoverflow.com/a/5607650/10005095
                std::stringstream ss(user_list);
                // in case UserListTokens is not deleted
                // Cannot use the commentted way, leading to "free(): invalida pointer"
                // auto loc = std::make_shared<UserListTokens>();
                ss.imbue(std::locale(std::locale(), new UserListTokens()));
                std::istream_iterator<std::string> begin(ss);
                std::istream_iterator<std::string> end;
                std::vector<std::string> users(begin, end);
                group_infos.push_back((GroupInfo){
                    .name = sm[1],
                    .gid = gid,
                    .users = std::move(users)
                });
            }
        }
    }
    return group_infos;
}
}
