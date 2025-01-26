#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"
#include <iostream>

class Client
{
private:
	int Fd;
	std::string IPadd; //-> client ip address
public:
	Client(){};
	int GetFd(){return Fd;}

	void SetFd(int fd){Fd = fd;}
	void setIpAdd(std::string ipadd){IPadd = ipadd;}
};

#endif