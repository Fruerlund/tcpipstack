#ifndef INC_NETENT_H
#define INC_NETENT_H

#include "common-defines.h"

#define MAC_LENGTH 6
#define NETENT_BUFFER_SIZE (4096)

typedef struct netent_t {

    uint32_t addr;  // IP
    char addr_string[128];
    uint8_t mac[6]; // MAC

} netent_t;

#include "netent.h"


struct netent_t * netent_alloc(char *ip, char *mac);
void netent_init(struct netent_t *ent, char *ip, char *mac);
int netent_send(char *data, size_t length);
int netent_recieve(struct netent_t *stack);
void netent_free(struct netent_t *netent);

#endif