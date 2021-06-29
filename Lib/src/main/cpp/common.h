//
// Created by panda on 2021/6/25.
//

#ifndef XMMAP_COMMON_H
#define XMMAP_COMMON_H

#include <android/log.h>
#include <jni.h>
#include <chrono>

using std::chrono::milliseconds;
using std::chrono::system_clock;

#define TAG    "【xmmap】"
#define LOGD(...)    __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGI(...)    __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...)    __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...)    __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define NS XMMAP
#define NS_BEGIN namespace NS {
#define NS_END }

#define TIME_START duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
#define TIME_END(tag, start) ({  \
    auto end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); \
    LOGE("----->%s time cost: %llu", tag, (end - start)); \
})
#endif //XMMAP_COMMON_H
