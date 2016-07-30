#ifndef EPOLL_MANAGER_H
#define EPOLL_MANAGER_H

#include <sys/epoll.h>

#include "connection.h"

class EpollManager {
private:
	int _epoll_fd;
	int _listen_fd;
	epoll_event event;

	int accept_nonblocking();
	void epoll_add_descriptor(int fd_);
	void epoll_set_fd_readable(Connection *);
	void epoll_set_fd_writable(Connection *);
	void epoll_event_handler();
	void do_write(Connection *);
	void do_read(Connection *);

public:
	EpollManager(int listen_fd);
	~EpollManager();
	void start();
};

#endif