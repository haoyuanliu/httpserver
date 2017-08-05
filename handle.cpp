#include "handle.h"

int Handle::work(int connfd) {
    char buf[MAX_BUFFER_SIZE];
    int recv_bytes;

    if(m_recvBuf.find(connfd) == m_recvBuf.end()) {
        m_recvBuf[connfd] = "";
    } else if (m_recvBuf[connfd].size() >= MAX_RECV_LEN) {
        printf("m_recvBuf[%d] overflow\n", connfd);
        m_recvBuf[connfd].clear();
    }

    while(true) {
        bzero(buf, MAX_BUFFER_SIZE);
        errno = 0;
        recv_bytes = recv(connfd, buf, MAX_BUFFER_SIZE, MSG_DONTWAIT);
        if (recv_bytes > 0) {
            m_recvBuf[connfd].append(buf, recv_bytes);
        } else if (recv_bytes == 0) {
            //TCP连接关闭，TCP的FIN会触发EPOLLIN事件
            printf("client close the connection\n");
            close(connfd);
            m_recvBuf.erase(connfd);
            return -1;
        } else {
            if (errno == EAGAIN) {
                break;
            } else {
                perror("read error");
                close(connfd);
                m_recvBuf.erase(connfd);
                return -1;
            }
        }
    }

    int ret;
    while(1) {
        httpRequest request = httpRequest(buf);
        httpResponse response;
        ret = dealRequest(request, response);
    }
    return 0;
}

int Handle::dealRequest(httpRequest req, httpResponse &res) {
    std::cout << res.toString() << std::endl;
    return 0;
}
