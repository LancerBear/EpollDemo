#ifndef __MY_EPOLL_H__
#define __MY_EPOLL_H__

#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <queue>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MesgNode.h"
using namespace std;

#define MAX_CLIENT_EVENT 100
#define MAX_BUF_LEN 100

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)



// 创建MyEpoll时启动新线程监听epoll事件，有事件时写入消息到消息队列中
class MyEpoll {
private:
    unsigned short portNum; // 监听的端口号
    pthread_t threadID;
    struct epoll_event ev;
    queue<MesgNode> *pq;

    int setNonBlock();
    int initListenSocket(int *socket);
    int dealClientMesg(char* buf);
    static void* threadRun(void *args);

public:
    MyEpoll(unsigned short port, queue<MesgNode>* q);
    ~MyEpoll();
};

#endif