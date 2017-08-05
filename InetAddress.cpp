#include "InetAddress.h"

static const in_addr_t InAddrAny = INADDR_ANY;
static const in_addr_t InAddrLoopback = INADDR_LOOPBACK;

InetAddress::InetAddress(std::string ip, uint16_t port) {
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);
}

InetAddress::InetAddress(uint16_t port, bool loopbackOnly) {
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    addr_.sin_addr.s_addr = htonl(ip);
    addr_.sin_port = htons(port);
}

std::string InetAddress::toIp() const {
    assert(addr_.sin_family == AF_INET);
    char buf[64];
    inet_ntop(AF_INET, &addr_.sin_addr.s_addr, buf, sizeof buf);
    return buf;
}

uint16_t InetAddress::toPort() const {
    uint16_t port = ntohs(addr_.sin_port);
    return port;
}

std::string InetAddress::toIpPort() const {
    assert(addr_.sin_family == AF_INET);
    char buf[64];
    inet_ntop(AF_INET, &addr_.sin_addr.s_addr, buf, sizeof buf);
    size_t len = strlen(buf);
    snprintf(buf+len, sizeof(buf)-len, ":%u", toPort());
    return buf;
}
