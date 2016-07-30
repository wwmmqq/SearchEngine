#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

Server::Server(const std::string &listen_address, int listen_port)
{
	std::cout << "creating server ... " std::endl;
	this->listen_fd = bind_listen_socket(listen_address, listen_port);
	this->event_handler.reset(new Epoll_Manager(listen_fd));
}

int Server::bind_listen_socket(std::string listen_address, int listen_port)
{
	sockaddr_in mysock;
	std::memset(&mysock, 0, sizeof(mysock));//初始化结构体
	mysock.sin_family = AF_INET;//ipv4
	inet_aton(listen_address.c_str(), &mysock.sin_addr);//inet_aton是一个改进的方法来将一个字符串IP地址转换为一个32位的网络序列IP地址
	mysock.sin_port = htons(listen_port);//设置端口

	int listen_fd;
	listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (listen_fd < 0) {
		std::cout << "could not create socket" << std::endl;
	}

	int t = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t))) {
		std::cout<< "could not set SO_REUSEADDR !" << std::endl;
	}

	if (bind(listen_fd, (struct sockaddr *) &mysock, sizeof(mysock))) {
		std::cout<<  "could not bind listen socket !" << std::endl;
	}

	if (listen(listen_fd, 48)) {
		std::cout<<  "could not listen on socket !" << std::endl;
	}
	//picked up by test_server.py to know server is ready
	//this line must be output after listen returns successfully
	std::cout << "listening on " << listen_address << ":" << listen_port << std::endl;
	return listen_fd;
}

void Server::run()
{
	std::cout << "running ..." << std::endl;
	event_handler->start();
}