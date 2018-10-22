//
// Created by Administrator on 2018/10/19.
//

#include "local/RawVideoDataQueue.h"

RawVideoDataQueue::RawVideoDataQueue() {
    mSizeLimit = 3;
    pthread_mutex_init(&mutex, NULL);
}

RawVideoDataQueue::~RawVideoDataQueue() {
    clear();
    pthread_attr_destroy(&mutex);
}

bool RawVideoDataQueue::putData(unsigned int timeStamp, unsigned char *data,
                                unsigned int dataLength, unsigned int width, unsigned int height) {
    pthread_mutex_lock(&mutex);
    if (bufferQueue.size() > mSizeLimit) {
        pthread_mutex_unlock(&mutex);
        return false;
    }
    RawVideoDataUnit *rawVideoDataUnit = new RawVideoDataUnit(dataLength, timeStamp, width, height);
    rawVideoDataUnit->setData(data, dataLength);
    bufferQueue.push_back(rawVideoDataUnit);
    pthread_mutex_unlock(&mutex);
    return true;
}

bool RawVideoDataQueue::getData(unsigned int &timeStamp, unsigned char *data,
                                unsigned int &dataLength, unsigned int &width,
                                unsigned int &height) {
    pthread_mutex_lock(&mutex);
    if (data == NULL | dataLength == 0) {
        pthread_mutex_unlock(&mutex);
        return false;
    }
    if (bufferQueue.size() > 0) {
        RawVideoDataUnit *rawVideoDataUnit = bufferQueue.front();
        if (dataLength >= rawVideoDataUnit->getDataLength()) {
            timeStamp = rawVideoDataUnit->getTimeStamp();
            width = rawVideoDataUnit->getWidth();
            height = rawVideoDataUnit->getHeight();
            dataLength = rawVideoDataUnit->getDataLength();

            memcpy(data, rawVideoDataUnit->getData(), dataLength);
            delete rawVideoDataUnit;
            bufferQueue.pop_front();
            pthread_mutex_unlock(&mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

void RawVideoDataQueue::clear() {
    pthread_mutex_lock(&mutex);
    while (bufferQueue.size() > 0) {
        RawVideoDataUnit *rawVideoDataUnit = bufferQueue.front();
        delete rawVideoDataUnit;
        bufferQueue.pop_front();
    }
    pthread_mutex_unlock(&mutex);
}

void RawVideoDataQueue::setSizeLimit(unsigned size) {
    mSizeLimit = size;
}

int RawVideoDataQueue::getSize() {
    return bufferQueue.size();
}