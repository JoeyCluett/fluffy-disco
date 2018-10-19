#include "TcpServer.h"
#include <iostream>

TcpServer::TcpServer(const std::string& hostname, int portno) {
    this->hostname = hostname;
    this->portno = portno;
}

void TcpServer::setConnectionCallback(void(*callback)(int)) { 
    this->connect_callback = callback; 
}

auto TcpServer::getHostname(void) -> std::string& { 
    return this->hostname; 
}

int TcpServer::getPortNumber(void) { 
    return this->portno; 
}

void TcpServer::start(int max_connections) {
    int option = 1;
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if(this->sockfd < 0) {
        std::cerr << "TcpServer: ERROR opening socket" << std::endl;
        std::cerr << "  Error: " << errno << std::endl << "    " << strerror(errno) << std::endl;
        exit(1);
    }

    // zero out structure
    memset((char*)&serv_addr, 0, sizeof(serv_addr));

    // configure serv_addr
    serv_addr.sin_family      = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(portno);

    // the big guy here
    if(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "TcpServer: ERROR binding socket" << std::endl;
        std::cerr << "  Error: " << errno << std::endl << "    " << strerror(errno) << std::endl;
        exit(1);
    }

    if(listen(sockfd, max_connections) < 0) {
        std::cerr << "TcpServer: ERROR on listen()" << std::endl;
        std::cerr << "  Error: " << errno << std::endl << "    " << strerror(errno) << std::endl;
        exit(1);
    }

    clilen = sizeof(cli_addr);

    // for as long as there are available connections
    for(;;) {
        int newsockfd = accept(sockfd, (sockaddr*)&cli_addr, &clilen);

        if(newsockfd < 0) {
            // not great, but not a deal breaker either
            std::cerr << "TcpServer: ERROR on accept" << std::endl;
            std::cerr << "  Error: " << errno << std::endl << "    " << strerror(errno) << std::endl;
        } else {
            if(connect_callback) { // if it has been specified, this will be non-zero
                connect_callback(newsockfd);
            } else
                std::cerr << "TcpServer: Connection Callback not specified\n";
        }
    }

}