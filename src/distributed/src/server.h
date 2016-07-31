#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server
{
private:
	int listen_fd;
	//add your members here
	std::unique_ptr<Epoll_Manager> event_handler;
	
	int bind_listen_socket(std::string listen_address, int listen_port);
public:
	Server(const std::string &listen_address, int listen_port);
	~Server();
	void run();
};

#include "server.cc"
#endif