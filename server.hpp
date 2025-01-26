#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"


#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h>
#include <csignal>

#define COLOR_RED "\e[1;31m"
#define COLOR_WHITE "\e[0;37m"
#define COLOR_GREEN "\e[1;32m"
#define COLOR_YELLOW "\e[1;33m"


class Client;

class Server
{
private:
	int serverPort;
	std::string password;
	int ServerSocket; //-> server socket file descriptor
	static bool signalFlag;
	std::vector<Client> clientList;
	std::vector<struct pollfd> pollDescriptors;
public:
	Server();

	void initializeServer(int serverPort, std::string password);
	void CreateSocket();
	void AcceptClient();
	void handleIncomingData(int fd);

	static void signalHandler(int signalNumbre);
	
	void closeAllConnections();
	void removeClient(int fd);
};

#endif