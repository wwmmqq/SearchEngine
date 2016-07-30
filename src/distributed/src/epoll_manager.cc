#include <sys/epoll.h>
#include <iostream>

#include "epoll_manager.h"

EpollManager::EpollManager(int listen_fd):listen_fd_(listen_fd)
{
	epoll_fd = epoll_create1(0);
	if(epoll_fd < 0)
		std::cerr << "epoll create error !\n";
	this->event.data.fd = nullptr;
	this->event.events = EPOLLIN | EPOLLET; //边沿触发
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &this->event);
}

int EpollManager::accept_nonblocking()
{
	sockaddr_in peer_addr;
	socklen_t peer_addr_size = sizeof(peer_addr);
	std::memset(&peer_addr, 0, peer_addr_size);
	/*
   * Accept new connections in non-blocking mode for use with edge-triggered epoll
   * it's ok if accept returns -1, we will use it to stop looking for new connections
   */
   int peer_fd = accept4(listen_fd, (struct sockaddr *) &peer_addr, &peer_addr_size, SOCK_NONBLOCK);
   return peer_fd;
}

void EpollManager::epoll_add_descriptor(int fd_)
{
	this->event.data.ptr = new Connection(fd_);
	this->event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_, &event)
}


void EpollManager::epoll_set_fd_readable(Connection *connection)
{
	event.data.ptr = connection;
	event.events = EPOLLET | EPOLLIN | EPOLLONESHOT;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, connection->get_descriptor(), &event);
}

void EpollManager::epoll_set_fd_writable(Connection *connection)
{
	event.data.ptr = connection;
	event.events = EPOLLET | EPOLLOUT | EPOLLONESHOT;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, connection->get_descriptor(), &event);
}

void EpollManager::epoll_event_handler()
{

}

void EpollManager::do_write(Connection *)
{

}

void EpollManager::do_read(Connection *)
{

}

void EpollManager::start()
{
	while(true) {
		epoll_event_handler();
	}
}










