#include "utils.h"

/**
 * https://github.com/saminiir/level-ip/blob/master/src/utils.c
 */

int run_cmd(char *cmd, ...) {

    va_list arguments;
    va_start(arguments, cmd);
    char buf[1024];
    vsnprintf(buf, 1024, cmd, arguments);
    va_end(arguments);

    system(buf);

    return 0;

}


void hexdump(char *buffer, size_t len) {
    for(int i = 0; i < len; i++) {
        printf("%02X ", buffer[i]);
        if ( i % 16 == 15) {
            printf("\n");
        }
    }

    printf("\n\n");
}



uint16_t checksum(void *addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */

    register uint32_t sum = 0;
    uint16_t * ptr = addr;

    while( count > 1 )  {
        /*  This is the inner loop */
        sum += * ptr++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if( count > 0 )
        sum += * (uint8_t *) ptr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}
