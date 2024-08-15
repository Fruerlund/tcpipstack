#include "common-defines.h"
#include "logging.h"
#include "network.h"
#include "arp.h"
#include "netent.h"


static char* dev;
static char *ipaddr = "10.0.0.1";       // IP and MAC for created TAP interface.
static char *iproute = "10.0.0.0/24";
static char stack_mac[6] = {0x0, 0x15, 0x5D, 0xBA, 0x14, 0x4C};
static char *stack_ip = "10.0.0.5";

static struct netent_t *stack;
int tapfd = 0;


char * getstackip(void) {
    return stack_ip;

}
char * getstackmac(void) {
    return stack_mac;
}


int main(int argc, char **argv) {

    // TAP Interface
    dev = calloc(10, 1);
    tapfd = tun_alloc(dev);    
    if(tapfd < 0) {
        exit(0);
    }

    tun_init(dev, ipaddr, iproute);

    // Network Entity
    netent_init(stack, stack_ip, stack_mac);

    // Routing

    // ARP
    arp_initcache();

    // TCP

    // CLI
    while(true) {
        netent_recieve(stack);
    }

    // Cleanup
    netent_free(stack);
    return 0;
}