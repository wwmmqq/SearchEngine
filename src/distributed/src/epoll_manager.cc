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
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, connection->get_descriptor(), &event);
}

void EpollManager::epoll_set_fd_writable(Connection *connection)
{
	event.data.ptr = connection;
	event.events = EPOLLET | EPOLLOUT | EPOLLONESHOT;
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, connection->get_descriptor(), &event);
}

void EpollManager::epoll_event_handler()
{
	std::vector<epoll_event> events_list(64);

	int nready = epoll_wait(epoll_fd, &*events_list.begin(), static_cast<int>(events_list.size()), -1);
	if(-1 == nready){
	//Timeout of -1 means epoll_wait will block until any monitored descriptors change
		continue;
	}

	if(0 == nready)
		continue;

	for(int i = 0; i < nready; ++i) {
		if(events_list[i].data.ptr == nullptr) {
			while(true) {
				int peer_fd = accept_nonblocking();
				if (peer_fd == -1)
					break;
				epoll_add_descriptor(peer_fd);
			}
		} else {
			Connection *connection = reinterpret_cast<Connection *>(events_list[i].data.ptr);
			if(events_list[i].events & EPOLLOUT) {
				do_write(connection);
			} else if(events_list[i].events & EPOLLOUT) {
				do_read(connection);
			} else {
				continue;
			}
		}
	} 

}

void EpollManager::do_write(Connection *connection)
{
	switch (connection->write_to_socket()) {
		case 1://write didn't finish
			epoll_set_fd_writable(connection);
			break;
		case 2://write was successful, read next
			 epoll_set_fd_readable(connection);
			break;
		case 3://error
			delete connection;
			break;
		default:
			break;
	}
}

void EpollManager::do_read(Connection *connection)
{
	switch (connection->read_from_socket()) {
		case 1://read was successful, write next
			epoll_set_fd_writable(connection);
			break;
		case 2://read didn't finish
			epoll_set_fd_readable(connection);
			break;
		case 3://read error or client disconnected
			delete connection;
		default:
			break;
	}
}

void EpollManager::start()
{
	while(true) {
		epoll_event_handler();
	}
}










