#ifndef __JJC__CLIENT__H__
#define __JJC__CLIENT__H__

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

class TcpClient {
private:
	int socketfd, portno;
	std::string hostname;
	sockaddr_in serv_addr;
	hostent* server;

public:
	TcpClient(const std::string& hostname, const std::string& port_number);
	TcpClient(const std::string& hostname, int port_number);
	TcpClient(void);

	// initialization of the socket happens here:
	void initSocket(const std::string& hostname, int port_number);

	// read/write chars into buffer
	int readBuffer(char* buf, int num);
	int writeBuffer(char* buf, int num);

	// file descriptor for this TcpClient
	int getFd(void);
};

#endif // __JJC__CLIENT__H__