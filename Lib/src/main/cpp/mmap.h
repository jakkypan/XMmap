//
// Created by panda on 2021/6/25.
//

#ifndef XMMAP_MMAP_H
#define XMMAP_MMAP_H

#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "common.h"

NS_BEGIN

typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u3;
typedef unsigned long u4;

#define RESIZE(a) (((a) / PAGE_SIZE + 1) * PAGE_SIZE)
const u1 MAGIC[] = {0x11, 0x12, 0x13, 0x14, 0x1B, 0x01}; //ascii 最后一位版本号

/**
 * mmap文件头，可以存放文件实际大小、mmap的版本号等
 */
typedef struct {
    u1 magic[sizeof(MAGIC)/sizeof(MAGIC[0])];
    u4 real_size;
    u2 append;
} mmap_header;

typedef struct {
    int file_fd;
    mmap_header *header;
    u4 buffer_size;
    u1 *buffer;
} mmap_info;

void open_mmap(const char *file_path, mmap_info *info, bool append);
bool write_mmap(mmap_info *info, const u1 *data, size_t data_size);
bool check_create_header(const u1 *buffer, size_t size, mmap_header *header);

NS_END
#endif //XMMAP_MMAP_H
