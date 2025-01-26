#include "Client.hpp"
#include "server.hpp"

int main(int ac,char **argv)
{
	if(ac != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
	}
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		int port = atoi(argv[1]);
		ser.ServerInit(port, argv[2]); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}