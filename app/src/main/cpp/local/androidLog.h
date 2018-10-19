//
// Created by Administrator on 2018/10/16.
//

#ifndef VIDEOPLAYPROJECT_ANDROIDLOG_H
#define VIDEOPLAYPROJECT_ANDROIDLOG_H

#include <android/log.h>


#define  LOG_TAG    "videoplayer"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#endif //VIDEOPLAYPROJECT_ANDROIDLOG_H