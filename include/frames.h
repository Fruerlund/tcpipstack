#ifndef INC_FRAMES_H
#define INC_FRAMES_H

#include "common-defines.h"

#include <linux/if_ether.h>


/**
 * 
 */
typedef struct ethernet_header {

    unsigned char destmac[6];
    unsigned char srcmac[6];
    uint16_t etherType;

} __attribute__((packed)) ethernet_header;


/**
 * 
 */
typedef struct ip_header {
    uint8_t version : 4;            // Version
    uint8_t ihl : 4;                // Internet header length
    uint8_t tos;                    // Type of service.
    uint16_t len;                   // Total length of IP datagram. Maximum length is 655535. Larger IP datagrams are split into smaller datagrams.
    uint16_t id;                    // Index field of datagram used for assembly.
    uint16_t flags : 3;             // Flags: Datagram can be fragmented, last fragment or more fragments coming
    uint16_t frag_offset : 13;      // Fragment offset indicates position of a fragment in a datrgram.
    uint8_t ttl;                    // Datagrams life time. Set to 64.
    uint8_t proto;                  // Data in datagrams 16(UDP) or 6(TCP)
    uint16_t csum;                  // Checksum
    uint32_t saddr;                 // Source address
    uint32_t daddr;                 // Destination address
} __attribute__((packed)) ip_header;


/**
 * 
 */
typedef struct arp_header
{
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed)) arp_header;


/**
 * 
 */
typedef struct icmp_header {

    uint8_t type;
    uint8_t code;
    uint16_t csum;

} __attribute__((packed)) icmp_header;

/*
*
*/

/**
 * 
 */
typedef struct icmp_echo {

    icmp_header header;
    uint16_t id;
    uint16_t seq;

} __attribute__((packed)) icmp_echo;

ethernet_header *createethernetheader(char *data);
void displayethernetheader(ethernet_header *header);
void displayarpheader(char *buffer);

#endif