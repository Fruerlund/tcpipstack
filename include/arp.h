#ifndef INC_ARP_H
#define INC_ARP_H

#include "common-defines.h"

#define ARP_ETHERNET    0x0001
#define ARP_IPV4        0x0800
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002

#define BITMAP_SIZE 4
#define ARP_CACHE_MAX ( 32 * BITMAP_SIZE )   // Number of maximum ARP Entries in our cache. (32 * 4)
#define ARP_HW_TYPE 0x1
#define ARP_PT_TYPE 0x8

#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

typedef struct arp_entry {

    uint32_t entryid;
    uint32_t ip;
    uint8_t mac[6];


} arp_entry;

extern arp_entry *arptable[ARP_CACHE_MAX];

void arp_clearcache(void);

int arp_addentry(uint32_t ip, uint8_t *mac);

int arp_deleteentry(uint32_t entryid);

arp_entry *arp_getentrybyid(uint32_t entryid);

void arp_initcache(void);

void arp_displaycache(void);

void arp_displayentry(arp_entry *entry);

arp_entry * arp_lookupcache(uint32_t ip);

void arp_sendreply(char *packet, size_t length);

void arp_handlearp(char *packet, size_t length);



#endif