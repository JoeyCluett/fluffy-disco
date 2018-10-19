#ifndef __JJC__TCP__SERVER__H__
#define __JJC__TCP__SERVER__H__

#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// libs needed for tcp server communication
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

class TcpServer {
private:
    std::string hostname = "";
    int portno;

    int sockfd;            // every server socket connection needs a file descriptor
    sockaddr_in serv_addr; // server address
    sockaddr_in cli_addr;  // client address
    socklen_t clilen;      // client length

    void(*connect_callback)(int) = 0;

public:
    TcpServer(const std::string& hostname, int portno);
    void start(int max_connections);
    void setConnectionCallback(void(*callback)(int) = 0);
    auto getHostname(void) -> std::string&;
    int getPortNumber(void);
};

#endif // __JJC__TCP__SERVER__H__
