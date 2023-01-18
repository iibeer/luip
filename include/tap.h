#ifndef __TAP_H
#define __TAP_H

#define TUNTAPDEV "/dev/net/tun"

extern void unset_tap(void);
extern int set_tap(void);
extern void delete_tap(int tapfd);
extern int alloc_tap(char *dev);
extern int gethwaddr_tap(int tapfd, unsigned char *ha);
extern int getname_tap(int tapfd, char *name);
extern int getipaddr_tap(char *name, unsigned int *ipaddr);
extern int setipaddr_tap(char *name, unsigned int ipaddr);
extern int getmtu_tap(char *name, int *mtu);
extern int setup_tap(char *name);
extern int setdown_tap(char *name);
extern int setflags_tap(char *name, unsigned short flags, int set);
extern int setnetmask_tap(char *name, unsigned int netmask);
extern int setpersist_tap(int fd);

#endif