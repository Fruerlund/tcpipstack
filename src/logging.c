#include "logging.h"

void log_info(char *s) {
    time_t now;
    time(&now);
    printf("%s", ctime(&now));
    printf("[INFO]: %s\n", s);
}

void log_warn(char *s) {
    time_t now;
    time(&now);
    printf("%s", ctime(&now));
    printf("[WARN]: %s %s\n", s, strerror(errno));
}

void log_debug(char *s) {
    time_t now;
    time(&now);
    printf("%s", ctime(&now));
    printf("[DEBUG]: %s\n", s);
}