//
// Created by Administrator on 2018/10/15.
//
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};
#include "local/videoInfo.h"

//获取视频长度
long getVideoDuration(AVFormatContext * avFormatContext){
    if(avFormatContext->duration != AV_NOPTS_VALUE){
        int hours, mins, secs, us;
        int64_t duration = avFormatContext->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        mins = secs / 60;
        secs %= 60;
        hours = mins/ 60;
        mins %= 60;
        printf("%02d:%02d:%02d.%02d\n", hours, mins, secs, (100 * us) / AV_TIME_BASE);
        return duration;
    }
    return 0;
}