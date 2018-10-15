//
// Created by Administrator on 2018/10/15.
//
#include "local/callJavaUtil.h"

void setVideoDuration(JNIEnv * env, long duration){

    const char* method_class_from_java = "com/linhao/video/MainActivity";
    // const char * method_name_from_java = "error";
    jclass cls_str_id = env->FindClass(method_class_from_java);

    if (cls_str_id==NULL) {
        return;
    }
    jmethodID m_Java_Func = env->GetMethodID(cls_str_id, "setVideoDuration", "(J)V");

    if(duration != 0 && m_Java_Func!=NULL) {
        env->CallStaticVoidMethod(cls_str_id, m_Java_Func,duration);
    }
}