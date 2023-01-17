#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <linux/if_tun.h>

#include "netif.h"
#include "tap.h"

static int skfd;

int setpersist_tap(int fd)
{
    /* if EBUSY, we donot set persist to tap */
    if (!errno && ioctl(fd, TUNSETPERSIST, 1) < 0) {
        perror("ioctl TUNSETPERSIST");
        return -1;
    }
    return 0;
}

void setnetmask_tap(char *name, unsigned int netmask)
{
    struct ifreq ifr = {};
    struct sockaddr_in *saddr;

    strcpy(ifr.ifr_name, name);
    saddr = (struct sockaddr_in *)&ifr.ifr_netmask;
    saddr->sin_family = AF_INET;
    saddr->sin_addr.s_addr = netmask;
    if (ioctl(skfd, SIOCSIFNETMASK, (void *)&ifr) < 0) {
        close(skfd);
        perror("socket SIOCSIFNETMASK");
    }
    fprintf(stderr, "set Netmask: "IPFMT, ipfmt(netmask));
}

void setflags_tap(char *name, unsigned short flags, int set)
{
    struct ifreq ifr = {};

    strcpy(ifr.ifr_name, name);
    /* get original flags */
    if (ioctl(skfd, SIOCGIFFLAGS, (void *)&ifr) < 0) {
        close(skfd);
        perror("socket SIOCGIFFLAGS");
    }
    /* set new flags */
    if (set)
        ifr.ifr_flags |= flags;
    else
        ifr.ifr_flags &= ~flags & 0xffff;
    if (ioctl(skfd, SIOCSIFFLAGS, (void *)&ifr) < 0) {
        close(skfd);
        perror("socket SIOCGIFFLAGS");
    }
}

void setdown_tap(unsigned char *name)
{
    setflags_tap(name, IFF_UP | IFF_RUNNING, 0);
}

void setup_tap(unsigned char *name)
{
    setflags_tap(name, IFF_UP | IFF_RUNNING, 1);
}

void getmtu_tap(unsigned char *name, int *mtu)
{
    struct ifreq ifr = {};

    strcpy(ifr.ifr_name, (char *)name);
    /* get net order hardware address */
    if (ioctl(skfd, SIOCGIFMTU, (void *)&ifr) < 0) {
        close(skfd);
        perror("ioctl SIOCGIFHWADDR");
    }
    *mtu = ifr.ifr_mtu;
}

void setipaddr_tap(unsigned char *name, unsigned int ipaddr)
{
    struct ifreq ifr = {};
    struct sockaddr_in *saddr;

    strcpy(ifr.ifr_name, (char *)name);
    saddr = (struct sockaddr_in *)&ifr.ifr_addr;
    saddr->sin_family = AF_INET;
    saddr->sin_addr.s_addr = ipaddr;
    if (ioctl(skfd, SIOCSIFADDR, (void *)&ifr) < 0) {
        close(skfd);
        perror("socket SIOCSIFADDR");
    }
}

void getipaddr_tap(unsigned char *name, unsigned int *ipaddr)
{
    struct ifreq ifr = {};
    struct sockaddr_in *saddr;

    strcpy(ifr.ifr_name, (char *)name);
    if (ioctl(skfd, SIOCGIFADDR, (void *)&ifr) < 0) {
        close(skfd);
        perror("socket SIOCGIFADDR");
    }
    saddr = (struct sockaddr_in *)&ifr.ifr_addr;
    *ipaddr = saddr->sin_addr.s_addr;
}

void getname_tap(int tapfd, unsigned char *name)
{
    struct ifreq ifr = {};
    if (ioctl(tapfd, TUNGETIFF, (void *)&ifr) < 0)
        perror("ioctl SIOCGIFHWADDR");
    strcpy((char *)name, ifr.ifr_name);
}

void gethwaddr_tap(int tapfd, unsigned char *ha)
{
    struct ifreq ifr;
    memset(&ifr, 0x0, sizeof(ifr));
    /* get net order hardware address */
    if (ioctl(tapfd, SIOCGIFHWADDR, (void *)&ifr) < 0)
        perror("ioctl SIOCGIFHWADDR");
    hwacpy(ha, ifr.ifr_hwaddr.sa_data);
}

int alloc_tap(char *dev)
{
    struct ifreq ifr;
    int tapfd;

    tapfd = open(TUNTAPDEV, O_RDWR);
    if (tapfd < 0) {
        perror("open");
        return -1;
    }

    memset(&ifr, 0x0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev)
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    /*
     * create a new tap device
     * if created already, just bind tun with file
     */
    if (ioctl(tapfd, TUNSETIFF, (void *)&ifr) < 0) {
        perror("ioctl TUNSETIFF");
        close(tapfd);
        return -1;
    }

    return tapfd;
}

void delete_tap(int tapfd)
{
    if (ioctl(tapfd, TUNSETPERSIST, 0) < 0)
        return;
    close(tapfd);
}

void set_tap(void)
{
    skfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (skfd < 0)
        perror("socket PF_INET");
}

void unset_tap(void)
{
    close(skfd);
}