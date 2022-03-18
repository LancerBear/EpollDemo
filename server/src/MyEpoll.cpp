#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include "MyEpoll.h"

MyEpoll::MyEpoll(unsigned short port, queue<MesgNode>* q) {
    this->portNum = port;
    this->pq = q;
    pthread_create(&threadID, NULL, threadRun, (void*)this);
}

MyEpoll::~MyEpoll() {
    
}

int MyEpoll::setNonBlock() {
    // if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
    //     return -1;
    // }
    return 0;
}

int MyEpoll::initListenSocket(int* ls) {
    int res = 0;
    int listenSocket = *ls;
    // 先生成一个socketfd
    // AF_INET IPv4
    // SOCK_STREAM tcp
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        ERR_EXIT("Add listen socket err.");
    }

    // 设置socket opt
    int opt = 1;
    res = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (res < 0) {
        ERR_EXIT("set socket opt SO_REUSEADDR err.");
    }
    res = setsockopt(listenSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    if (res < 0) {
        ERR_EXIT("set socket opt SO_REUSEADDR err.");
    }

    // 设置socket要监听的端口号
    struct sockaddr_in serverAddr = {};
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(this->portNum); // 监听的端口号
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 固定传参，本地IP

    // bind socketfd和监听端口的sockaddr_in
    res = bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (res < 0) {
        ERR_EXIT("bind socket with sockaddr_in err.");
    }

    // 开始监听
    res = listen(listenSocket, SOMAXCONN);
    if (res < 0) {
        ERR_EXIT("listen start err.");
    }

    return 0;
}

int MyEpoll::dealClientMesg(char* buf) {
    printf("dealClient mesg. code = %d\r\n", GET_MESG_CODE_FROM_BUF(buf));
    MesgNode node(GET_MESG_CODE_FROM_BUF(buf), GET_MESG_FROM_BUF(buf));
    pq->push(node);

    return 0;
}

void* MyEpoll::threadRun(void* args) {
    int res = 0;
    MyEpoll *p = (MyEpoll*)args;
    int listenSocket;
    int epollFd;
    vector<struct epoll_event> clientEvent(MAX_CLIENT_EVENT);

    // 初始化监听socket fd，设置监听的端口，bind，listen
    res = p->initListenSocket(&listenSocket);
    if (res < 0 || listenSocket < 0) {
        ERR_EXIT("init listen socket err.");
    }

    // 创建epoll
    epollFd = epoll_create(1);
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    
    ev.data.fd = listenSocket;
    ev.events = EPOLLIN | EPOLLET; // EPOLLET边缘触发，EPOLLEL水平触发
    epoll_ctl(epollFd, EPOLL_CTL_ADD, listenSocket, &ev); // epoll add增加监听epoll

    while (1) {
        // epoll_wait阻塞，有事件返回事件的个数
        int fdNum = epoll_wait(epollFd, &*clientEvent.begin(), MAX_CLIENT_EVENT, -1);
        if (fdNum < 0) {
            // 失败
            ERR_EXIT("epoll wait err.");
            break;
        } else if (fdNum == 0) {
            // 理论上epoll_wait阻塞，在超时的情况才会返回0，此时进行下一个训话，再次epoll_wait
            continue;
        }

        // 遍历进来的事件，一个个处理
        for (int i = 0; i < fdNum; i++) {
            if (clientEvent[i].data.fd == listenSocket && clientEvent[i].events & EPOLLIN) {
                // 事件的fd和listenSocket一样，说明有新的客户端连入
                // 此时需要通过accept新建一个和client通信的socket
                sockaddr_in clientAddr;
                socklen_t clientSize = sizeof(sockaddr_in);
                int clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientSize);

                // 把clientSocket也加入epoll监听
                memset(&ev, sizeof(struct epoll_event), 0);
                ev.data.fd = clientSocket;
                ev.events = EPOLLIN;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &ev);
                printf("client connected socket = %d\r\n", clientSocket);
            } else if (clientEvent[i].events & EPOLLIN) {
                // 有EPOLLIN事件，可能是客户端有数据发送过来，或者是客户端连接断开
                int connFd = clientEvent[i].data.fd;
                char buf[MAX_BUF_LEN] = {0};
                int readSize = read(connFd, buf, MAX_BUF_LEN);
                if (readSize < 0) {
                    // 读数据出错
                    ERR_EXIT("read err.");
                    break;
                } else if (readSize == 0) {
                    // 缓冲区没有数据也出发了EPOLLIN，说明客户端socket端口，把监听的事件从epoll中删除，并close fd
                    memset(&ev, 0, sizeof(epoll_event));
                    ev.data.fd = connFd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, connFd, &ev);
                    close(connFd);
                } else {
                    // 缓冲区有数据，说明客户端有数据发过来
                    p->dealClientMesg(buf);
                }
            }
        }
    }

    close(epollFd);

    return NULL;
}