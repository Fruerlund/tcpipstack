#ifndef INC_UTILS_H
#define INC_UTILS_H

#include "common-defines.h"

int run_cmd(char *cmd, ...);
void hexdump(char *buffer, size_t len);
uint16_t checksum(void *addr, int count);
#endif