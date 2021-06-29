//
// Created by panda on 2021/6/25.
//

#ifndef XMMAP_AUTORELEASEBYTEARRAY_H
#define XMMAP_AUTORELEASEBYTEARRAY_H

#include "common.h"

NS_BEGIN
class AutoReleaseByteArray {
private:
    JNIEnv *mEnv;
    const jbyteArray mJniArray;
    jbyte *mCBytes;

public:
    AutoReleaseByteArray(JNIEnv *mEnv, const jbyteArray array) : mEnv(mEnv), mJniArray(array) {
        mCBytes = mEnv->GetByteArrayElements(mJniArray, NULL);
    }

    virtual ~AutoReleaseByteArray() {
        mEnv->ReleaseByteArrayElements(mJniArray, mCBytes, 0);
    }

    jbyte *getCBytes() {
        return mCBytes;
    }
};
NS_END

#endif //XMMAP_AUTORELEASEBYTEARRAY_H
