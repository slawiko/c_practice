#ifndef DIGEST_STRUCTURE_H
#define DIGEST_STRUCTURE_H

#include <zconf.h>

struct file_checksum {
    uLong checksum;
    char* path;
};

#endif //DIGEST_STRUCTURE_H
