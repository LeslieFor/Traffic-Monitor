#include <stdio.h>
#include <memory.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>

int get_mac(char *mac_str, int len)
{
    struct ifreq  ifr;
    struct ifconf ifc;
    int success = 0;
    char buf[1024]       = {0x00};
    unsigned char mac[6] = {0x00};

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        printf("socket error\n");
        return -1;
    };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it)
    {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
        {
            // don't count loopback
            if (! (ifr.ifr_flags & IFF_LOOPBACK))
            {
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0)
                {
                    success = 1;
                    break;
                }
            }
        }
    }

    if (success != 1)
    {
        return -1;
    }

    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    snprintf(mac_str, len, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return 0;
}
