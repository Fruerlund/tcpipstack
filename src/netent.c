#include "netent.h"
#include "packethandling.h"
#include "network.h"

static char netent_buffer[NETENT_BUFFER_SIZE] = { 0 };

/**
 * 
 */
struct netent_t * netent_alloc(char *ip, char *mac) {

    printf("NETENT_INIT\n");

    struct netent_t *ent = (struct netent_t *)malloc(sizeof(struct netent_t));
    memset(ent, '\x00', sizeof(struct netent_t));
    strncpy(ent->addr_string, ip, strlen(ip));
    memcpy(&ent->mac, mac, MAC_LENGTH);
    ent->addr = 0;

    return ent;

}


/**
 * 
 */
void netent_init(struct netent_t *ent, char *ip, char *mac) {

    ent = netent_alloc(ip, mac);

}


/**
 * 
 */
int netent_send(char *packet, size_t length) {

    tun_write(length, packet);

}


/**
 * 
 */
int netent_recieve(struct netent_t *stack) {

    ssize_t bytesRead = tun_read(NETENT_BUFFER_SIZE, netent_buffer);
    
    if( bytesRead < 0) {
        printf("Failed to read from tun device");
    }

    packethandlerparse(netent_buffer, bytesRead);

}

/**
 *
 */
void netent_free(struct netent_t *netent) {

    printf("NETENT_FREE\n");
    if(netent != NULL) {
        free(netent);
    }
}