#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"
#include <iostream>

class Client
{
private:
	int Fd;
	std::string IPaddress;
public:
	Client(){};
	int getFileDescriptor(){return Fd;}

	void SetFd(int fd){Fd = fd;}
	void setIpAdd(std::string ipaddress){IPaddress = ipaddress;}
};

#endif