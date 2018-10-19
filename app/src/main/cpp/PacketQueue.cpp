//
// Created by Administrator on 2018/10/16.
//

#include "local/PacketQueue.h"
#include "local/androidLog.h"

//初始化队列
void packet_queue_init(PacketQueue *packetQueue) {
    memset(packetQueue, 0, sizeof(PacketQueue));
    pthread_mutex_init(&packetQueue->mutex, NULL);
    pthread_cond_init(&packetQueue->cond, NULL);
}

//清除队列
void packet_queue_clear(PacketQueue *packetQueue) {
    pthread_mutex_lock(&packetQueue->mutex);
    AVPacketList *tmp = NULL;
    tmp = packetQueue->first_pkt;
    while (packetQueue->size > 0 && tmp != NULL) {

        packetQueue->first_pkt = tmp->next;
        if (tmp->pkt.data) {
            av_free(&tmp->pkt);
            tmp->pkt.data = NULL;
        }
        av_free(tmp);
        tmp = packetQueue->first_pkt;
        packetQueue->nb_packets--;
    }
    packetQueue->nb_packets = 0;
    packetQueue->first_pkt = NULL;
    packetQueue->last_pkt = NULL;
    packetQueue->size = 0;
    pthread_mutex_unlock(&packetQueue->mutex);
}

//销毁队列
void packet_queue_destory(PacketQueue *packetQueue) {
    pthread_mutex_lock(&packetQueue->mutex);
    AVPacketList *temp = NULL;
    temp = packetQueue->first_pkt;
    while (packetQueue->nb_packets > 0 && temp != NULL) {
        packetQueue->first_pkt = temp->next;
        av_free(temp);
        temp = packetQueue->first_pkt;
        packetQueue->nb_packets--;
    }
    packetQueue->first_pkt = NULL;
    packetQueue->last_pkt = NULL;
    packetQueue->nb_packets = 0;
    packetQueue->size = 0;
    if (&packetQueue->mutex) {
        pthread_mutex_destroy(&packetQueue->mutex);
    }
    if (&packetQueue->cond) {
        pthread_cond_destroy(&packetQueue->cond);
    }
    pthread_mutex_unlock(&packetQueue->mutex);
}

//数据进队列
int packet_queue_put(PacketQueue *packetQueue, AVPacket *pkt) {

    AVPacketList *packetList;
    if (av_dup_packet(pkt) < 0) {
        return -1;
    }
    packetList = (AVPacketList *) av_malloc(sizeof(AVPacketList));
    if (packetList == NULL) {
        return -1;
    }
    packetList->pkt = *pkt;
    packetList->next = NULL;
    pthread_mutex_lock(&packetQueue->mutex);
    if (packetQueue->last_pkt == NULL) {
        packetQueue->first_pkt = packetList;
    } else {
        packetQueue->last_pkt->next = packetList;
    }
    packetQueue->last_pkt = packetList;
    packetQueue->nb_packets++;
    packetQueue->size += packetList->pkt.size;
    pthread_cond_signal(&packetQueue->cond);
    pthread_mutex_unlock(&packetQueue->mutex);
    return 0;
}

//数据出队列
int packet_queue_get(PacketQueue *packetQueue, AVPacket *pkt, int block) {

    AVPacketList *packetList;
    int ret;
    pthread_mutex_lock(&packetQueue->mutex);
    for (;;) {

        packetList = packetQueue->first_pkt;
        if (packetList != NULL) {

            packetQueue->first_pkt = packetList->next;
            if (!packetQueue->first_pkt){
                 packetQueue->last_pkt = NULL;
            }
            packetQueue->nb_packets--;
            packetQueue->size -= packetList->pkt.size;
            *pkt = packetList->pkt;
            free(packetList);
            ret = 1;
            break;
        } else{
            ret = 0;
            break;
        }
    }
    pthread_mutex_unlock(&packetQueue->mutex);
    return ret;
}