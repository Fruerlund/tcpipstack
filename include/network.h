#ifndef INC_NETWORK_H
#define INC_NETWORK_H

#include "common-defines.h"

int tun_alloc(char *);
ssize_t tun_read(ssize_t size, char *data);
ssize_t tun_write(ssize_t size, char *data);
void tun_init(char *dev, char *ip, char *route);
char * getipaddr(void);
char * getiproute(void);
char *getmac(void);
struct in_addr getipaddruint32(void);

#endif