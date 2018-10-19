#include <tcp/Client.h>

#include <errno.h>
#include <string.h>
#include <iostream>

TcpClient::TcpClient(void) { ; }

TcpClient::TcpClient(const std::string& hostname, const std::string& port_number) {
	initSocket(hostname, atoi(port_number.c_str()));
}

TcpClient::TcpClient(const std::string& hostname, int port_number) {
	initSocket(hostname, port_number);
}

// where the real work gets done
void TcpClient::initSocket(const std::string& hostname, int port_number) {
	// assign for class vars
	this->hostname = hostname;
	this->portno = port_number;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0) {
		std::cerr << "ERROR opening socket client" << std::endl;
		exit(1);
	}

	server = gethostbyname(hostname.c_str());
	if(server == NULL) {
		std::cerr << "ERROR, no such host" << std::endl;
		std::cerr << "    " << strerror(errno) << std::endl;
		exit(1);
	}

	// configure server information
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port_number);

	// connect to the server, quit if fails
	if(connect(socketfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << "ERROR connecting to [" << hostname << ", " << port_number << "]\n";
		std::cerr << "    " << strerror(errno) << std::endl;
		exit(1);
	}
}

int TcpClient::readBuffer(char* buf, int num) {
	return read(socketfd, buf, num);
}

int TcpClient::writeBuffer(char* buf, int num) {
	return write(socketfd, buf, num);
}

int TcpClient::getFd(void) {
	return socketfd;
}
