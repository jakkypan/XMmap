//
// Created by panda on 2021/6/25.
//

#include "mmap.h"
#include "common.h"
#include <sys/stat.h>

void NS::open_mmap(const char *file_path, mmap_info *info, bool append) {
    if (strlen(file_path) == 0 || info == nullptr) {
        LOGE("invalid arguments at `open_mmap` function");
        return;
    }

    int file_fd = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (file_fd < 0) {
        LOGE("failed to open mmap");
        return;
    }

    info->file_fd = file_fd;
    info->buffer = nullptr;
    info->buffer_size = 0;
    struct stat buffer_file_stat{};
    if (fstat(file_fd, &buffer_file_stat) < 0) {
        LOGE("read file error");
        return;
    }
    auto buffer_size = (size_t) buffer_file_stat.st_size;
    // 小于0需要给个最低的大小，就是1page
    // 如果是覆盖写，就清除原来的大小，再设置成1page
    if (buffer_size <= 0 || !append) {
        buffer_size = PAGE_SIZE;
        ftruncate(file_fd, buffer_size);
        lseek(file_fd, 0, SEEK_SET);
    }
    u1 *buffer = (u1 *) mmap(0, buffer_size, PROT_WRITE | PROT_READ, MAP_SHARED, file_fd, 0);
    if (buffer == MAP_FAILED || buffer == nullptr) {
        LOGE("mmap error");
        return;
    }
    auto *header = new mmap_header;
    bool hasExit = check_create_header(buffer, buffer_size, header);
    if (!hasExit) {
        memcpy(header->magic, MAGIC, sizeof(MAGIC));
        header->real_size = 0;
        header->append = append ? 1 : 0;
        memcpy(buffer, header, sizeof(mmap_header));
    } else if (header->append != append) {
        header->append = append ? 1 : 0;
        memcpy(buffer, header, sizeof(mmap_header));
    }
    info->header = header;
    info->buffer = buffer;
    info->buffer_size = buffer_size;
}

bool NS::write_mmap(mmap_info *info, const u1 *data, size_t data_size) {
    size_t total_size = data_size + sizeof(mmap_header);
    if (info->buffer_size <= total_size) {
        LOGD("original size %ld, original address %x", info->buffer_size, (u4) info->buffer);
        u4 resized_size = RESIZE(total_size);
        ftruncate(info->file_fd, (off_t) resized_size);
        info->buffer = (u1 *) mremap(info->buffer, info->buffer_size, resized_size, MREMAP_MAYMOVE);
        info->buffer_size = resized_size;
        LOGE("resized buffer %ld, new address %x", info->buffer_size, (u4) info->buffer);
        if (info->buffer == MAP_FAILED) {
            LOGE("remap failed");
            info->buffer = nullptr;
            info->buffer_size = 0;
            return false;
        }
    }
    if (info->buffer == nullptr) {
        LOGE("buffer is null, ignore！");
        return false;
    }
    auto *header = new mmap_header;
    bool success = check_create_header(info->buffer, info->buffer_size, header);
    if (!success) {
        LOGE("invalid header, stop mmap write");
        return false;
    }
    u4 last_size = header->real_size;
    if (header->append == 1) {
        header->real_size += data_size;
    } else {
        // 覆盖数据少于历史数据，清除多余的数据
//        if (last_size > data_size) {
//            memset(info->buffer + sizeof(mmap_header), '-', info->buffer_size);
//        }
        header->real_size = data_size;
        last_size = 0;
    }
    // 重新写文件头
    memcpy(info->buffer, header, sizeof(mmap_header));
    // 写文件内容（追加的方式需要加上上次写数据时的位置，覆盖的话就last_size就是0）
    memcpy(info->buffer + sizeof(mmap_header) + last_size, data, data_size);
    return true;
}

bool NS::check_create_header(const u1 *buffer, size_t size, mmap_header *header) {
    if (buffer == nullptr || size < sizeof(mmap_header)) {
        return false;
    }

    int result = memcmp(buffer, MAGIC, sizeof(MAGIC));
    if (result == 0) {
        memcpy(header, buffer, sizeof(mmap_header));
    }
    return result == 0;
}
