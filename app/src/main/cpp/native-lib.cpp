#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "local/native-lib.h"
#include "local/videoInfo.h"
#include "local/callJavaUtil.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

#define  LOG_TAG    "videoplayer"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

void error(JNIEnv *env,char errrorMsg[50]);
void success(JNIEnv *env,char errrorMsg[50]);
void finish(JNIEnv *env,char errrorMsg[50]);
void callBack(JNIEnv * env, const char *msg, const char * methorName);

extern "C" JNIEXPORT jstring JNICALL
Java_com_linhao_video_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

uint8_t *buffer;
AVFrame *frame;
AVFrame *pFrameRGBA;
AVCodecContext *pCodecCtx;
AVFormatContext *pFormatCtx;

extern "C"
JNIEXPORT void JNICALL
Java_com_linhao_video_MainActivity_setSurface(JNIEnv *env, jobject instance, jobject surface,
                                              jstring path_) {


    const char *filePath = env->GetStringUTFChars(path_, 0);
    av_register_all();

    //获取编码上下文对象
    pFormatCtx = avformat_alloc_context();

    //打开视频文件
    if (avformat_open_input(&pFormatCtx, filePath, NULL, NULL) != 0) {
        const char * msg = "Couldn't open file";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Couldn't open file:%s\n", filePath);
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return;
    }

    //查找视频流得信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        const char * msg = "Couldn't find stream information.";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Couldn't find stream information.");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return;
    }
    //获取视频长度
    long duration = getVideoDuration(pFormatCtx);
    setVideoDuration(env,duration);

    int videoStreamIndex = -1, i;

    //查找视频流得位置
    for (i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO &&
            videoStreamIndex < 0) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        const char * msg = "Didn't find a video stream.";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Didn't find a video stream.");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return; // Didn't find a video stream
    }

    pCodecCtx = pFormatCtx->streams[videoStreamIndex]->codec;

    //找到解压器
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        const char * msg = "Codec not found.";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Codec not found.");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return; // Codec not found
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        const char * msg = "Could not open codec.";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Could not open codec.");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return; // Could not open codec
    }

    LOGD("111111111");
    // 获取native window
    ANativeWindow *aNativeWindow = ANativeWindow_fromSurface(env, surface);
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;

    // 设置native window的buffer大小,可自动拉伸
    ANativeWindow_setBuffersGeometry(aNativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;

    //得到原始帧
    frame = av_frame_alloc();
    // 用于渲染
    pFrameRGBA = av_frame_alloc();

    if (frame == NULL || pFrameRGBA == NULL) {
        const char * msg = "Could not allocate video frame.";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Could not allocate video frame.");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return;
    }
    LOGD("2222222");
    //计算出需要为buffer申请内存空间
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);

    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    // buffer中数据就是用于渲染的,且格式为RGBA
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         videoWidth, videoHeight, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(videoWidth, videoHeight, pCodecCtx->pix_fmt,
                                                videoWidth, videoHeight, AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR, NULL, NULL, NULL);
    LOGD("333333333333");
    if (sws_ctx==NULL){
        const char * msg = "Could not get SwsContext";
        const char * name = "error";
        callBack(env,msg,name);
        LOGD("Could not get SwsContext");
        env->ReleaseStringUTFChars(path_, filePath);
        free_malloc();
        return;
    }
    int frameFinish;
    AVPacket packet;
    LOGD("5555555555");
    const char * msg = "success";
    const char * name = "success";
    callBack(env,msg,name);

    LOGD("44444444444");
    while (av_read_frame(pFormatCtx, &packet) >= 0) {

        if (packet.stream_index == videoStreamIndex) {
            //开始解压视频流
            avcodec_decode_video2(pCodecCtx, frame, &frameFinish, &packet);
            //并不是一次就可以解码出一帧得
            if (frameFinish) {
                ANativeWindow_lock(aNativeWindow, &window_buffer, 0);

//                if (checkExc(env)){
//                    JNU_ThrowByName(env,"java/lang/Exception", "exception from jni: jni exception happened at p1111");
//                }
                // 格式转换
                sws_scale(sws_ctx, (const uint8_t *const *) frame->data, frame->linesize, 0,
                          videoHeight, pFrameRGBA->data, pFrameRGBA->linesize);
//                if (checkExc(env)){
//                    JNU_ThrowByName(env,"java/lang/Exception", "exception from jni: jni exception happened at p2222");
//                }
                // 获取stride
                uint8_t *dst = (uint8_t *) window_buffer.bits;
                int dstStride = window_buffer.stride * 4;
                uint8_t *src = (pFrameRGBA->data[0]);
                int srcStride = pFrameRGBA->linesize[0];
                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }
                ANativeWindow_unlockAndPost(aNativeWindow);
            }
        }
        av_packet_unref(&packet);
    }
    //釋放內存
    free_malloc();
    env->ReleaseStringUTFChars(path_, filePath);
    return;
}

//釋放內存
void free_malloc() {

    if (buffer != NULL) {
        av_free(buffer);
    }
    if (frame != NULL) {
        av_free(frame);
    }

    if (pFrameRGBA != NULL) {
        av_free(pFrameRGBA);
    }

    if (pCodecCtx != NULL) {
        avcodec_close(pCodecCtx);
    }

    if (pFormatCtx != NULL) {
        avformat_close_input(&pFormatCtx);
    }
}

int checkExc(JNIEnv * env){

    if (env->ExceptionCheck()){
        env->ExceptionDescribe(); // writes to logcat
        env->ExceptionClear();
        return 1;
    }
    return -1;
}

void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg){
// 查找异常类
    jclass cls = env->FindClass(name);
    /* 如果这个异常类没有找到，VM会抛出一个NowClassDefFoundError异常 */
    if (cls != NULL) {
        env->ThrowNew(cls, msg);  // 抛出指定名字的异常
    }
    /* 释放局部引用 */
    env->DeleteLocalRef(cls);
}


void callBack(JNIEnv * env, const char * msg , const char * methorName){
    const char* method_class_from_java = "com/linhao/video/MainActivity";
   // const char * method_name_from_java = "error";
    jclass cls_str_id = env->FindClass(method_class_from_java);
     if (cls_str_id==NULL){
         return;
     }
    jmethodID m_Java_Fun = env->GetMethodID(cls_str_id, methorName, "(Ljava/lang/String;)V");
    if(msg != NULL && m_Java_Fun !=NULL){
        jstring msgData = env->NewStringUTF(msg);
        env->CallStaticVoidMethod(cls_str_id, m_Java_Fun,msgData);
    }
}