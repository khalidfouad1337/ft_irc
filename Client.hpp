#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal>

class Client //-> class for client
{
private:
	int Fd; //-> client file descriptor
	std::string IPadd; //-> client ip address
public:
	Client(){}; //-> default constructor
	int GetFd(){return Fd;} //-> getter for fd

	void SetFd(int fd){Fd = fd;} //-> setter for fd
	void setIpAdd(std::string ipadd){IPadd = ipadd;} //-> setter for ipadd
};

#endif