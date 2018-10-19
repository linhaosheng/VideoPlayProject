//
// Created by Administrator on 2018/10/12.
//

#ifndef VIDEOPLAYPROJECT_NATIVE_LIB_H
#define VIDEOPLAYPROJECT_NATIVE_LIB_H

void free_malloc();

int checkExc(JNIEnv * env);

void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg);

#endif //VIDEOPLAYPROJECT_NATIVE_LIB_H