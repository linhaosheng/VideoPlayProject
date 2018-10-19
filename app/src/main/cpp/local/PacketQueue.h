//
// Created by Administrator on 2018/10/16.
//


#ifndef VIDEOPLAYPROJECT_PACKETQUEUE_H
#define VIDEOPLAYPROJECT_PACKETQUEUE_H

#include "pthread.h"
#include "string.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};


typedef struct PacketQueue {
    AVPacketList * first_pkt, *last_pkt;
    int nb_packets;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    PacketQueue() : first_pkt(NULL),last_pkt(NULL),nb_packets(0),size(0){}
}PacketQueue;

void  packet_queue_init(PacketQueue * packetQueue);

void packet_queue_clear(PacketQueue * packetQueue);

void packet_queue_destory(PacketQueue * packetQueue);

int packet_queue_put(PacketQueue * packetQueue,AVPacket * pkt);

int packet_queue_get(PacketQueue * packetQueue,AVPacket * pkt,int block);
#endif //VIDEOPLAYPROJECT_PACKETQUEUE_H