//
// Created by Administrator on 2018/10/19.
//

#ifndef VIDEOPLAYPROJECT_RAWVIDEODATAUNIT_H
#define VIDEOPLAYPROJECT_RAWVIDEODATAUNIT_H


#include <string.h>

class RawVideoDataUnit {

public:
    RawVideoDataUnit(int size, unsigned int timestamp, int width = 0, int height = 0);

    ~RawVideoDataUnit();

    void setData(unsigned char *data, int dataLength);

    unsigned char *getData();

    unsigned int getTimeStamp();

    unsigned int getDataLength();
    unsigned getWidth();

    unsigned int getHeight();

private:
    unsigned int mTimeStamp;

    unsigned int mBufferSize;

    unsigned int mWidth;

    unsigned int mHeight;

    unsigned char *mBuffer;
};

#endif //VIDEOPLAYPROJECT_RAWVIDEODATAUNIT_H