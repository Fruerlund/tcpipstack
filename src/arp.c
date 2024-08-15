#include "arp.h"
#include "frames.h"
#include "network.h"
#include "netent.h"

uint32_t arpnumberentries = 0;

arp_entry *arptable[ARP_CACHE_MAX];

uint32_t arpbitmap[BITMAP_SIZE] = { 0 };

/**
 * 
 */
void arp_initcache(void) {

    printf("ARP_INIT\n");
    memset(arpbitmap, '\x00', BITMAP_SIZE);

}


/**
 * Bitmap
 */
int findFreeSpot(void) {
    for(int i = 0; i < BITMAP_SIZE; i++) {
        if(arpbitmap[i] != 0xFFFFFFFF) {
            for(int j = 0; j < 32; j++) {
                if( !(arpbitmap[i] & ( 1 << j))) {
                    return i * 32 + j;
                }
            }
        }
    }
    return -1;
}


/*
*
*/
void setBit(uint32_t index) {

    arpbitmap[index / 32 ] |= ( 1 << (index % 32));    

}

/**
 * 
 */
void unsetBit(uint32_t index) {
    
    arpbitmap[index / 32 ] &= ~( 1 << (index % 32));    

}

/*
*
*/
bool isBitSet(uint32_t index) {
    
    return arpbitmap[index / 32] & (1 << (index % 32));
}


/**
 * 
 */
void arp_clearcache(void) {
    for(int i = 0; i < ARP_CACHE_MAX; i++) {
        if( isBitSet(i) ) {
            free(arptable[i]);
        }
    }
    memset(arpbitmap, '\x00', BITMAP_SIZE);
}
  


/**
 * 
 */
int arp_addentry(uint32_t ip, uint8_t *mac) {

    uint32_t id = findFreeSpot();

    if(id == -1) {
        return -1;
    }

    arp_entry *entry = (arp_entry *)malloc(sizeof(struct arp_entry));
    entry->entryid = id;
    entry->ip = ip;
    memcpy(entry->mac, mac, 6);
    arptable[id] = entry;

    setBit(id);

    return id;

}

/**
 * 
 */
int arp_deleteentry(uint32_t entryid) {

    if( isBitSet(entryid) ) {

        arptable[entryid]->entryid = -1;
        arptable[entryid]->ip = 0;
        memset(arptable[entryid]->mac, '\x00', 6);
        free(arptable[entryid]);
        unsetBit(entryid);

    }

    return entryid;

}

/*
 *
 */
arp_entry * arp_getentrybyid(uint32_t entryid) {
    if( isBitSet(entryid) ) {
        return arptable[entryid];
    }
}

/**
 * 
 */
void arp_displaycache(void) {
    for(int i = 0; i < ARP_CACHE_MAX; i++) {
        if( isBitSet(i) ) {
            arp_displayentry(arptable[i]);
        }
    }
}

/**
 *
 */
void arp_displayentry(arp_entry *entry) {
    printf("[%d]: ", entry->entryid);
    printf("MAC: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X", entry->mac[i]);
        if (i < 5) printf(":");
    }
    struct in_addr ip;
    ip.s_addr = entry->ip;
    printf(" -> %s\n", inet_ntoa(ip));

}

/*
*
*/
arp_entry * arp_lookupcache(uint32_t ip) {
    for(int i = 0; i < ARP_CACHE_MAX; i++) {
        if( isBitSet(i) ) {
            if( memcmp( &arptable[i]->ip, &ip, 4) == 0) {
                return arptable[i];
            }
        }
    }
    return NULL;
}



/*
*
*/
void arp_sendreply(char *packet, size_t length) {

    
    ethernet_header *eth = (ethernet_header *)(packet);
    arp_header *arp = (arp_header *) (packet + sizeof(struct ethernet_header) );

    char smac[6] = { 0 };
    memcpy(&smac, eth->srcmac, 6);
    uint32_t sip = arp->sip;

    memcpy(&eth->destmac, smac, 6);
    memcpy(&eth->srcmac, getstackmac(), 6);

    arp->dip = sip;

    char *ipString = getstackip();
    struct in_addr ip_addr;
    if (inet_pton(AF_INET, ipString, &ip_addr) != 1) {
        return;
    }

    memcpy(&arp->sip, &ip_addr.s_addr, 4);
    memcpy(&arp->dmac, &smac, 6);
    memcpy(&arp->smac, getstackmac(), 6);

    arp->opcode = htons(ARP_REPLY);

    displayethernetheader(eth);
    displayarpheader(packet);

    netent_send(packet, length);

    
}

/*
*
*/
void arp_handlearp(char *packet, size_t length) {

    arp_header *arp = (arp_header *) (packet + sizeof(struct ethernet_header) );


    if(arp->hwtype == htons(ARP_HW_TYPE)) {

        if(arp->protype == ARP_PT_TYPE) {

            if ( ( arp_lookupcache(arp->sip) ) == NULL ) {
                arp_addentry( arp->sip, arp->smac);
            }

            if( arp->opcode == htons(ARP_REQUEST)) {

                // Our IP address as string and bytes.
                char *ipString = getstackip();
                struct in_addr ip_addr;
                if (inet_pton(AF_INET, ipString, &ip_addr) != 1) {
                    return;
                }

                // Compare with target IP as bytes.
                if(memcmp(&arp->dip, &ip_addr.s_addr, 4) == 0) {
                    arp_sendreply(packet, length);
                }
            }
        }
    }
}