#include "network.h"
#include "logging.h"
#include "utils.h"

/**
 * 
 */
static int set_if_route(char *dev, char *cidr) {

    return run_cmd("ip route add dev %s %s", dev, cidr);

}


/**
 * 
 */
static int set_if_address(char *dev, char *cidr) {

    return run_cmd("ip address add dev %s local %s", dev, cidr);

}


/**
 * 
 */
static int set_if_up(char *dev) {

    return run_cmd("ip link set dev %s up", dev);

}


/**
 * 
 */
int tun_alloc(char *dev) {

    struct ifreq ifr;
    int fd, err;
    memset(&ifr, '\x00', sizeof(struct ifreq));

    if ( ( fd = open("/dev/net/tap", O_RDWR)) < 0) {
        log_warn("Cannot open TUN/TAP device");
        exit(1);
    }

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( ( err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        log_warn("Could not icotl tun");
    }

    strcpy(dev, ifr.ifr_name);
    log_info("Got TUN/TAP descriptor");
    log_info(dev);

    return fd;

}

/**
 * 
 */
ssize_t tun_read(ssize_t size, char *data) {

    ssize_t bytesRead = read(tapfd, data, size);

    if(bytesRead < 0) {
        log_warn("Failed to read from tun");
        return -1;
    }

    return bytesRead;
}


/**
 * 
 */
ssize_t tun_write(ssize_t size, char *data) {

    ssize_t bytesWritten = write(tapfd, data, size);

    if(bytesWritten < 0) {
        log_warn("Failed to write to tun");
        return -1;
    }

    return bytesWritten;

}

/**
 * 
 */
void tun_init(char *dev, char *ip, char *route) {

    log_info("TUN_INIT\n");
    log_info(dev);

    if (set_if_up(dev) != 0) {
        log_warn("ERROR when setting up if");
    }

    if (set_if_route(dev, route) != 0) {
        log_warn("ERROR when setting route for if");
    }

    if (set_if_address(dev, ip) != 0) {
        log_warn("ERROR when setting addr for if");
    }

    log_info("TUN/TAP Device setup complete");

}