//
// Created by panda on 2021/6/25.
//

#ifndef XMMAP_AUTORELEASESTRING_H
#define XMMAP_AUTORELEASESTRING_H

#include "common.h"

NS_BEGIN
class AutoReleaseString {
private:
    JNIEnv *mEnv;
    const jstring mJniStr;
    const char *mCStr;

public:
    AutoReleaseString(JNIEnv *env, const jstring jstr) : mEnv(env), mJniStr(jstr) {
        mCStr = mEnv->GetStringUTFChars(mJniStr, 0);
    }

    virtual ~AutoReleaseString() {
        mEnv->ReleaseStringUTFChars(mJniStr, mCStr);
    }

    const char *getCStr() noexcept {
        return mCStr;
    }
};
NS_END

#endif //XMMAP_AUTORELEASESTRING_H
