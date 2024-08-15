#include "frames.h"

/**
 * 
 */
ethernet_header *createethernetheader(char *data) {

    ethernet_header *header = (ethernet_header *)malloc(sizeof(ethernet_header));
    memset(header, '\x00', sizeof(struct ethernet_header));

    // Unpack data
    memcpy(header->destmac, data, 6);
    memcpy(header->srcmac, data+6, 6);
    memcpy(&header->etherType, data+12, sizeof(uint16_t));

    header->etherType = htons(header->etherType);



    return header;

}

/**
 * 
 */
void displayethernetheader(ethernet_header *header) {

    printf("[ETHERNET]: ");
    printf("[TYPE]: 0x%04X ", header->etherType);    
    for(int i = 0; i < 6; i++) {
        printf("%02X ", header->srcmac[i]);
    }
    printf(" --> ");
    for(int i = 0; i < 6; i++) {
        printf("%02X ", header->destmac[i]);
    }
    printf("\n");

}

/**
 * 
 */
void displayarpheader(char *buffer) {

    const char *ARP_TYPE[]= {
        "GARBAGE",
        "ARP_REQUEST",
        "ARP_REPLY"
    };

    arp_header *arp = (arp_header *) (buffer + sizeof(struct ethernet_header) );

    printf("[ARP]:\n");

    printf("  Hardware type: %d\n", ntohs(arp->hwtype));
    printf("  Protocol type: 0x%04X\n", ntohs(arp->protype));
    printf("  Hardware size: %d\n", arp->hwsize);
    printf("  Protocol size: %d\n", arp->prosize);
    printf("  Opcode: %d (%s)\n", ntohs(arp->opcode), ARP_TYPE[ntohs(arp->opcode)]);

    printf("  Sender MAC: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X", arp->smac[i]);
        if (i < 5) printf(":");
    }
    printf("\n");

    struct in_addr dip;
    struct in_addr sip;
    dip.s_addr = arp->dip;
    sip.s_addr = arp->sip;

    printf("  Sender IP: %s\n", inet_ntoa(sip));

    printf("  Target MAC: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X", arp->dmac[i]);
        if (i < 5) printf(":");
    }
    printf("\n");

    printf("  Target IP: %s\n", inet_ntoa(dip));

};