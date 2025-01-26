#include "server.hpp"


Server::Server()
{
ServerSocket = -1;
password = "";
serverPort = 4444;
}

bool Server::signalFlag = false;

void Server::signalHandler(int signalNumber)
{
	(void)signalNumber;
	std::cout << std::endl << "Signal received. Shutting down server." << std::endl;
	Server::signalFlag = true;
}

void	Server::closeAllConnections()
{
	for(size_t i = 0; i < clientList.size(); i++)
	{
		std::cout << COLOR_RED << "Client <" << clientList[i].getFileDescriptor() << "> disconnected." << COLOR_WHITE << std::endl;
		close(clientList[i].getFileDescriptor());
	}
	if (ServerSocket != -1)
	{
		std::cout << COLOR_RED << "Server socket <" << ServerSocket << "> closed." << COLOR_WHITE << std::endl;
		close(ServerSocket);
	}
}

void Server::AcceptClient()
{
	Client newClient;
	struct sockaddr_in clientAddress;
	struct pollfd NewPoll;
	socklen_t addressLength = sizeof(clientAddress);

	int clientSocket = accept(ServerSocket, (sockaddr *)&(clientAddress), &addressLength);
	if (clientSocket == -1)
		{std::cout << "Error: Failed to accept client connection." << std::endl; return;}

	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
		{
			std::cout << "Error: Failed to set socket to non-blocking mode." << std::endl;
			return;
		}

	NewPoll.fd = clientSocket;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	newClient.SetFd(clientSocket);
	newClient.setIpAdd(inet_ntoa((clientAddress.sin_addr)));
	clientList.push_back(newClient);
	pollDescriptors.push_back(NewPoll);

	std::cout << COLOR_GREEN << "New client connected. Socket: " << clientSocket << COLOR_WHITE << std::endl;
}

void Server::handleIncomingData(int fd)
{
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytesRead = recv(fd, buff, sizeof(buff) - 1 , 0);

	if(bytesRead <= 0)
	{
		std::cout << COLOR_RED << "Client <" << fd << "> disconnected." << COLOR_WHITE << std::endl;
		removeClient(fd);
		close(fd);
	}

	else
	{
		buff[bytesRead] = '\0';
		std::cout << COLOR_YELLOW << "Client <" << fd << "> sent: " << COLOR_WHITE << buff;
		// Mohamed here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}
void Server::removeClient(int fd)
{
	for(size_t i = 0; i < pollDescriptors.size(); i++)
	{
		if (pollDescriptors[i].fd == fd)
		{
			pollDescriptors.erase(pollDescriptors.begin() + i);
			break;
		}
	}
	for(size_t i = 0; i < clientList.size(); i++)
	{
		if (clientList[i].getFileDescriptor() == fd)
		{
			clientList.erase(clientList.begin() + i);
			break;
		}
	}

}

void Server::CreateSocket()
{
	int optionValue = 1;
	struct sockaddr_in serverAddress;
	struct pollfd NewPoll;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->serverPort); //-> convert the port to network byte order (big endian)

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(ServerSocket == -1)
		throw(std::runtime_error("faild to create socket"));

	if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	 if (fcntl(ServerSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(ServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(ServerSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = ServerSocket;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	pollDescriptors.push_back(NewPoll);
}

void Server::initializeServer(int serverPort, std::string password)
{
	this->serverPort = serverPort;
	this->password = password;
	CreateSocket();

	std::cout << COLOR_GREEN << "Server socket created. Listening on port: " << serverPort  << COLOR_WHITE << std::endl;
	std::cout << "Waiting for client connections..." << std::endl;

	while (Server::signalFlag == false)
	{
		if((poll(&pollDescriptors[0],pollDescriptors.size(),-1) == -1) && Server::signalFlag == false)
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < pollDescriptors.size(); i++)
		{
			if (pollDescriptors[i].revents & POLLIN)
			{
				if (pollDescriptors[i].fd == ServerSocket)
					AcceptClient();
				else
					handleIncomingData(pollDescriptors[i].fd);
			}
		}
	}
	closeAllConnections();
}

