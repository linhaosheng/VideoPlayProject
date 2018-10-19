//
// Created by Administrator on 2018/10/15.
//

#ifndef VIDEOPLAYPROJECT_CALLJAVAUTIL_H
#define VIDEOPLAYPROJECT_CALLJAVAUTIL_H
#include <jni.h>

void setVideoDuration(JNIEnv * env, long duration);

//设置当前播放进度
void setCurrentProgress(JNIEnv * env, long duration);

#endif //VIDEOPLAYPROJECT_CALLJAVAUTIL_H