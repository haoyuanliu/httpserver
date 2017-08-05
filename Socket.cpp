#include "Socket.h"

#define MAXCONN 1024

void setNonBlockAndCloseOnExec(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = fcntl(sockfd, F_SETFL, flags);

    flags = fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = fcntl(sockfd, F_SETFD, flags);
}

Socket::Socket() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
    close(sockfd_);
}

void Socket::bindAddress(InetAddress &addr) {
    bind(sockfd_, (struct sockaddr*)(addr.getAddr()), static_cast<socklen_t>(sizeof(struct sockaddr_in)));
}

void Socket::toListen() {
    listen(sockfd_, MAXCONN);
}

int Socket::toAccept(struct sockaddr_in *addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    int connfd = accept(sockfd_, (struct sockaddr*)addr, &addrlen);
    setNonBlockAndCloseOnExec(connfd);
    return connfd;
}

void Socket::setTcpNoDelay(bool on) {
    int optval  = on ? 1 : 0;
    setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
}
