#ifndef CONNECTION_H
#define CONNECTION_H

#include "kv_store.h"

class Connection {
private:
	int fd_;
	std::string read_buffer;
	std::shared_ptr<KV> _kv;
	int get_descriptor()
	{
		return fd_;
	};
public:
	Connection(int fd, std::shared_ptr<KV> kv):fd_(fd), _kv(kv) {};
	~Connection()
	{
		close(fd_);
	};
	uint8_t read_from_socket();
	uint8_t write_to_socket();
	void process_message();

};

#endif