#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include "InetAddress.h"

class Socket {
    public:
        Socket();
        explicit Socket(int sockfd) : sockfd_(sockfd) {}
        ~Socket();
        int get_fd() { return sockfd_; }

        void bindAddress(InetAddress &addr);
        void toListen();
        int toAccept(struct sockaddr_in *addr);

        void setTcpNoDelay(bool on);
        void setReuseAddr(bool on);
        void setReusePort(bool on);
        void setKeepAlive(bool on);
    private:
        int sockfd_;
};
