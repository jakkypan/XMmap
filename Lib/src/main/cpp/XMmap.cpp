#include <jni.h>
#include <string>
#include "common.h"
#include "AutoReleaseString.h"
#include "AutoReleaseByteArray.h"
#include "mmap.h"

using namespace NS;

extern "C"
JNIEXPORT jlong JNICALL
Java_com_panda_xmmap_XMmap_nativeInit(JNIEnv *env, jclass clazz, jstring file_path, jboolean append) {
    AutoReleaseString autoReleaseString(env, file_path);
    const char *c_file_path = autoReleaseString.getCStr();
    auto *info = new mmap_info;
    open_mmap(c_file_path, info, append);
    return reinterpret_cast<jlong>(info);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_panda_xmmap_XMmap_nativeWrite(JNIEnv *env, jclass clazz, jlong native_ptr,
                                       jbyteArray data) {
    auto start = TIME_START;
    if (native_ptr == 0) {
        LOGE("native ptr is zero");
        return false;
    }
    auto mInfo = reinterpret_cast<mmap_info *>(native_ptr);
    if (mInfo == nullptr) {
        LOGE("long cast error");
        return false;
    }
    jsize len = env->GetArrayLength(data);
    AutoReleaseByteArray byteArray(env, data);
    jbyte *native_data = byteArray.getCBytes();
    bool success = write_mmap(mInfo, (const u1 *) native_data, len);
    TIME_END("mmap write", start);
    return success;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_panda_xmmap_XMmap_nativeRead(JNIEnv *env, jclass clazz, jlong native_ptr) {
    auto start = TIME_START;
    if (native_ptr == 0) {
        LOGE("native ptr is zero");
        return nullptr;
    }
    auto mInfo = reinterpret_cast<mmap_info *>(native_ptr);
    if (mInfo == nullptr) {
        LOGE("long cast error");
        return nullptr;
    }
    auto *header = new mmap_header;
    bool success = check_create_header(mInfo->buffer, mInfo->buffer_size, header);
    if (!success) {
        LOGE("invalid header, this is not mmap file, stop read");
        return nullptr;
    }
    if (header->real_size <= 0 || mInfo->buffer_size <= 0) {
        return nullptr;
    }
    jbyteArray array = env->NewByteArray(header->real_size);
    // 从header结束的位置开始读取
    env->SetByteArrayRegion(array, 0, header->real_size,
            (const jbyte *) (mInfo->buffer + sizeof(mmap_header)));
    TIME_END("mmap read", start);
    return array;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_panda_xmmap_XMmap_nativeClose(JNIEnv *env, jclass clazz, jlong native_ptr) {
    if (native_ptr == 0) {
        LOGE("native ptr is zero");
        return false;
    }
    auto mInfo = reinterpret_cast<mmap_info *>(native_ptr);
    if (mInfo == nullptr) {
        LOGE("long cast error");
        return false;
    }
//    memset(mInfo->buffer, 0, mInfo->buffer_size);
    int unmap = munmap(mInfo->buffer, mInfo->buffer_size);
    int closed = close(mInfo->file_fd);
    return unmap==0&&closed==0;
}