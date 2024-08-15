#include "packethandling.h"
#include "frames.h"
#include "utils.h"
#include "arp.h"

void packethandlerparse(char *buffer, size_t length) {

    ethernet_header *header = createethernetheader(buffer);
    
    uint16_t type = header->etherType;

    switch(type) {

        case ETH_P_ARP:                 // 2054

            displayethernetheader(header); 
            displayarpheader(buffer);
            arp_handlearp(buffer, length);
            break;

        case ETH_P_IP:                  // 2048
            displayethernetheader(header); 
            printf("IP Packet\n");
            break;

        case ETH_P_IPV6:
            break;
            
        default:
            printf("Unimplemented packet\n");

    }

    printf("\n");

}
