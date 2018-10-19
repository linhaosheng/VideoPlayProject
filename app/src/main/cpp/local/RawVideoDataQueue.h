//
// Created by Administrator on 2018/10/19.
//

#ifndef VIDEOPLAYPROJECT_RAWVIDEODATAQUEUE_H
#define VIDEOPLAYPROJECT_RAWVIDEODATAQUEUE_H

#include <pthread.h>
#include <deque>
#include "RawVideoDataUnit.h"

class RawVideoDataQueue{
public:
    RawVideoDataQueue(void);

    ~RawVideoDataQueue(void);

    bool putData(unsigned int timeStamp, unsigned char * data,unsigned int dataLength,unsigned int width,unsigned int height);

    bool getData(unsigned int &timeStamp, unsigned char * data, unsigned int & dataLength, unsigned int & width,unsigned int &heigjt);

    void setSizeLimit(unsigned size);

    int getSize();

    void clear();

private:
    pthread_mutex_t mutex;

    std::deque<RawVideoDataUnit *> bufferQueue;

    int mSizeLimit;
};
#endif //VIDEOPLAYPROJECT_RAWVIDEODATAQUEUE_H