#include "util.h"

void *scalloc(size_t count, size_t size) {
    void *ptr = calloc(count, size);
    if (NULL == ptr) {
        log_err("Not enough memory available");
        exit(-1);
    }
    return ptr;
}

char *sstrndup(char *s, size_t size) {
    char *ptr = strndup(s, size);
    if (NULL == ptr) {
        log_err("Not enough memory available");
        exit(-1);
    }
    return ptr;
}

// Taken from http://stackoverflow.com/a/9444636
char *convert_to_upper(char *str){
    char *newstr, *p;
    for (p = newstr = strdup(str); *p != '\0'; ++p) {
        *p = toupper(*p);
    }

    return newstr;
}

