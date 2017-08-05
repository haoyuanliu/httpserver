#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include "Socket.h"
#include "httpRequest.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -1;
    }
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress server_addr = InetAddress(port, false);
    Socket server_sock = Socket();
    server_sock.bindAddress(server_addr);
    server_sock.toListen();
    server_sock.setTcpNoDelay(true);
    server_sock.setReuseAddr(true);
    server_sock.setReusePort(true);

    int connect_fd;
    char buf[4096];
    int size;
        bzero(&buf, sizeof buf);
        connect_fd = server_sock.toAccept(NULL);
        size = read(connect_fd, &buf, sizeof buf);
        cout << buf << endl;
        httpRequest req = httpRequest(buf);
        cout << req.getStrMethod() << endl;
        cout << req.getPath() << endl;
        cout << req.getVersion() << endl;
        map<string, string> headers = req.getMapHeaders();
        map<string, string>::iterator it = headers.begin();
        for ( ; it != headers.end(); ++it) {
            cout << it->first << ":" << it->second << endl;
        }
        cout << req.toString();
    return 0;
}
