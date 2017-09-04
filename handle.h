#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "httpRequest.h"
#include "httpResponse.h"
#include "threadpool.h"

#define MAX_BUFFER_SIZE 4096
#define MAX_RECV_LEN    20480

extern std::map<int, std::string> m_recvBuf;

class Handle {
    public:
        Handle() {}
        ~Handle() {}
    public:
        int work(int connfd);
    private:
        int dealRequest(httpRequest &req, httpResponse &res);
        std::string getType(std::string uri);
        std::string getPath(std::string path);
        std::string getContent(std::string path);
};
