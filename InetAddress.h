#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include <string>

class InetAddress {
    public:
        InetAddress(std::string ip, uint16_t port);
        explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false);
        explicit InetAddress(const struct sockaddr_in &addr) : addr_(addr) {}

        std::string toIp() const;
        uint16_t toPort() const;
        std::string toIpPort() const;
        struct sockaddr_in *getAddr() {
            return static_cast<struct sockaddr_in*>(&addr_);
        }
    private:
        struct sockaddr_in addr_;
};
