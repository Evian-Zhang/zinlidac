#include "system.h"
#include "util.h"
#ifdef __linux__
#include "unistd.h"
#endif

#include <string>
#include <vector>
#include <regex>
#include <fstream>

namespace libzinlidac {
// throws a `SpecialError` if name of the user logged in cannot be determined
LoginUserInfo get_associated_user() {
    #ifdef __linux__
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
    #endif
}

// throws a `FileReadError` if cannot open /etc/passwd
// throws a `SpecialError` if cannot parse uid or groupid to integer
std::vector<UserInfo> get_users() {
    #ifdef __linux__
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
            if (sm.length() == 7) {
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
    #endif
}
}
