#include "connection.h"

uint8_t Connection::read_from_socket()
{
	ssize_t bytes_received;
	char buf[4096];
	/*
   * read everything from the socket and store it until it returns:
   * 0, peer disconnected
   * -1, there's nothing left to read (EAGAIN) or error
   */
   while ((bytes_received = read(this->fd_, buf, sizeof(buf) )) > 0) {
		read_buffer.append(buf, bytes_received);
		std::memset(buf, 0, 4096); //reset the buffer for the next read
	}

	if(-1 == bytes_received) {
		return 4;//Terminate the connection because of error
	}

	if(0 == bytes_received) {
		return 3;//Terminate the connection because of disconnect
	}

	if (read_buffer.back() == '\n') {
		process_message();
		return 1;
	} else {
		return 3;
	}
	return 2;
}

void Connection::process_message()
{

}


uint8_t Connection::write_to_socket()
{
	return 2;
}