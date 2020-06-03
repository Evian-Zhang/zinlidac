#include "system.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <regex>
#ifdef __linux__
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netpacket/packet.h>
#endif

namespace libzinlidac {
static const struct {
    unsigned short int family_code;
    const char *family_string;
} NetworkFamilyTypes[] = {
    {AF_PACKET, "AF_PACKET"},
    {AF_INET, "AF_INET"},
    {AF_INET6, "AF_INET6"},
    {AF_BLUETOOTH, "AF_BLUETOOTH"}
};

// throws a `SpecialError` if getifaddrs failed
std::vector<NetworkInterfaceInfo> get_network_interfaces() {
    #ifdef __linux__
    std::vector<NetworkInterfaceInfo> interface_infos;
    struct ifaddrs *interface_address;
    if (getifaddrs(&interface_address) == -1) {
        throw system::SpecialError("getifaddrs failed");
    }
    while (interface_address != NULL) {
        if (interface_address->ifa_addr == NULL) {
            interface_address = interface_address->ifa_next;
            continue;
        }
        auto sa_family = interface_address->ifa_addr->sa_family;
        std::string network_family = "Unknown";
        for (unsigned int i = 0; i < sizeof(NetworkFamilyTypes) / sizeof(NetworkFamilyTypes[0]); i++) {
            if (NetworkFamilyTypes[i].family_code == sa_family) {
                network_family = NetworkFamilyTypes[i].family_string;
                break;
            }
        }
        std::string address = "Unknown";
        // see `man getifaddrs`
        unsigned int socket_len = 0;
        switch (sa_family) {
            case AF_INET: socket_len = sizeof(struct sockaddr_in); break;
            case AF_INET6: socket_len = sizeof(struct sockaddr_in6); break;
            default: break;
        }
        if (sa_family == AF_INET || sa_family == AF_INET6) {
            char host[NI_MAXHOST];
            if (getnameinfo(interface_address->ifa_addr, socket_len, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == 0) {
                address = host;
            } else {
                throw system::SpecialError("getnameinfo failed");
            }
        } else if (sa_family == AF_PACKET) {
            // see https://stackoverflow.com/q/6762766/10005095
            char macp[INET6_ADDRSTRLEN];
            struct sockaddr_ll *s = (struct sockaddr_ll*)(interface_address->ifa_addr);
            int len = 0;
            for (int i = 0; i < 6; i++) {
                len += sprintf(macp + len, "%02X%s", s->sll_addr[i], i < 5 ? ":" : "");
            }
            address = macp;
        }
        interface_infos.push_back((NetworkInterfaceInfo){
            .name = interface_address->ifa_name,
            .network_family = std::move(network_family),
            .address = std::move(address)
        });
        interface_address = interface_address->ifa_next;
    }
    freeifaddrs(interface_address);
    return interface_infos;
    #endif
}

#ifdef __linux
// throws a SpecialError if failed to execute lsof -Ua
std::vector<LsofResult> get_lsof__Ua() {
    std::vector<LsofResult> lsof_results;
    FILE *command = popen("lsof -Ua", "r");
    if (command == NULL) {
        throw system::SpecialError("Failed to execute `lsof -Ua`");
    }
    std::string line_string;
    auto regex = std::regex("(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t.*?\t.*?\t(.*)");
    std::smatch sm;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, command)) != -1) {
        line_string = line;
        if (std::regex_match(line_string, sm, regex)) {
            if (sm.length() == 8) {
                unsigned int pid;
                try {
                    pid = std::stoi(sm[2]);
                } catch (...) {
                    continue;
                }
                lsof_results.push_back((LsofResult){
                    .command = sm[1],
                    .pid = pid,
                    .user = sm[2],
                    .fd = sm[3],
                    .type = sm[4],
                    .device = sm[5],
                    .name = sm[6]
                });
            }
        }
    }
    return lsof_results;
}
#endif
}
