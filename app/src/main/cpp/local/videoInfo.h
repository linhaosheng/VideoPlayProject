//
// Created by Administrator on 2018/10/15.
//
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};
#ifndef VIDEOPLAYPROJECT_VIDEOINFO_H
#define VIDEOPLAYPROJECT_VIDEOINFO_H

#endif //VIDEOPLAYPROJECT_VIDEOINFO_H

//获取视频长度
long getVideoDuration(AVFormatContext * avFormatContext);
