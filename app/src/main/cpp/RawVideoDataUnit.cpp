//
// Created by Administrator on 2018/10/19.
//
#include "local/RawVideoDataUnit.h"

RawVideoDataUnit::RawVideoDataUnit(int size, unsigned int timeStamp, int width, int height) :
        mTimeStamp(timeStamp), mBufferSize(size), mWidth(width), mHeight(height) {

    mBuffer = new u_char[size];
}

RawVideoDataUnit::~RawVideoDataUnit() {
    if (mBuffer != NULL) {
        delete[]mBuffer;
        mBuffer = NULL;
    }
}

void RawVideoDataUnit::setData(unsigned char *data, int dataLength) {
    if (data == NULL || dataLength == 0 || dataLength > mBufferSize) {
        return;
    }

    memcpy(mBuffer, data, dataLength);
    mBufferSize = dataLength;
}

unsigned char *RawVideoDataUnit::getData() {

    return mBuffer;
}

unsigned int RawVideoDataUnit::getDataLength() {
    return mBufferSize;
}

unsigned int RawVideoDataUnit::getHeight() {
    return mHeight;
}

unsigned int RawVideoDataUnit::getWidth() {
    return mWidth;
}

unsigned int RawVideoDataUnit::getTimeStamp() {
    return mTimeStamp;
}
