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
            break;
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
    httpRequest request(buf);
    httpResponse response;
    ret = dealRequest(request, response);
    std::string temp = response.toString();
    send(connfd, temp.c_str(), temp.size(), MSG_DONTWAIT);
    return 0;
}

int Handle::dealRequest(httpRequest &req, httpResponse &res) {
    std::cout << "Request:" << std::endl;
    std::cout << req.toString() << std::endl;
    res.setStatus(httpResponse::k200Ok);
    res.setCloseConnection(false);
    std::string path = getPath(req.getPath());
    std::cout << path << std::endl;
    res.setContentType(getType(path));
    res.setBody(getContent(path));
    std::cout << "Response:" << std::endl;
    std::cout << res.toString() << std::endl;
    return 0;
}

std::string Handle::getType(std::string uri) {
    std::string type;
    int size = uri.size();
    int dot = size - 1;
    while(dot >= 0 && uri[dot] != '.') {
        dot--;
    }
    if (dot == 0) {
        return "text/html";
    } else if (dot < 0) {
        return "text/html";
    } else {
        dot++;
        type.assign(uri, dot, size-dot);
        Util::toUpper(type);
        if ("HTML" == type)
            return "text/html";
        if ("JPEG" == type)
            return "image/jpeg";
        if ("HTM" == type)
            return "text/html";
        if ("CSS" == type)
            return "text/css";
        if ("PNG" == type)
            return "image/png";
        if ("JPG" == type)
            return "image/jpg";
        if ("GIF" == type)
            return "image/gif";
        if ("TXT" == type)
            return "text/plain";
        if ("JS" == type)
            return "text/javascript";
    }
    return "NULL";
}

std::string Handle::getPath(std::string path) {
    if (access(path.c_str(), F_OK) == -1) {
        return "../content/404.html";
    }
    if (access(path.c_str(), R_OK) == -1) {
        return "../content/403.html";
    }
    return path;
}

std::string Handle::getContent(std::string path) {
    std::cout << "Path: " << path << std::endl;
    std::ifstream ifs;
    ifs.open(path, std::ios::binary);
    if (!ifs.is_open()) {
        return "Error while opening file!";
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string res = oss.str();
    return res;
}
