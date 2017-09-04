#include <iostream>
#include <string>
#include <sstream>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>

#include "Socket.h"
#include "handle.h"

#define MAX_EPOLL_SIZE 1024

using namespace std;
std::map<int, string> m_recvBuf;
threadpool_t g_thread_pool;

Socket initSock(uint16_t port) {
    InetAddress server_addr = InetAddress(port, false);
    Socket server_sock = Socket();
    server_sock.setTcpNoDelay(true);
    server_sock.setReuseAddr(true);
    server_sock.setReusePort(true);
    server_sock.bindAddress(server_addr);
    server_sock.toListen();
    return server_sock;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -1;
    }

    threadpool_init(&g_thread_pool, 10);

    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    Socket server = initSock(port);
    int listenfd = server.get_fd();

    int epollfd = epoll_create(MAX_EPOLL_SIZE);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    int listenq = 1024;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        fprintf(stderr, "epoll set insertion error: fd = %d\n", listenfd);
        return -1;
    }
    cout << "http_server startup, port: " << port
         << ", max connection is: " << MAX_EPOLL_SIZE << endl;


    int nfds, connfd;
    int curfds = 1;
    struct epoll_event events[MAX_EPOLL_SIZE];
    InetAddress cli_addr;
    while(1) {
        cout << "Waiting ..." << endl;

        nfds = epoll_wait(epollfd, events, curfds, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            continue;
        }

        for(int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listenfd) {
                connfd = server.toAccept(cli_addr.getAddr());
                if (connfd < 0) {
                    perror("accept error");
                    continue;
                }

                printf("Accept from %s\n", cli_addr.toIpPort().c_str());
                if (curfds >= MAX_EPOLL_SIZE) {
                    fprintf(stderr, "too many connection, more than %d\n", MAX_EPOLL_SIZE);
                    close(connfd);
                    continue;
                }
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
                    fprintf(stderr, "Add socket %d to epoll failed: %s\n", connfd, strerror(errno));
                    return -1;
                }
                curfds++;
                continue;
            }

            Handle handle;
            if (handle.work(events[i].data.fd) < 0) {
                epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                curfds--;
            }
        }
    }
    return 0;
}
