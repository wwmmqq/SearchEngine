#include "server.h"

#include <iostream>

#include <signal.h>
#include <sys/prctl.h>
/*
a simple memory-only shared state key value server in C++11 on Linux
*/
int main(int argc, char **argv) {
  //kill this process if parent process exits
  ::prctl(PR_SET_PDEATHSIG, SIGKILL);

  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " " << "ipv4_listen_address port" << std::endl;
    return 1;
  }

  std::string addr{argv[1]};
  int port = std::atoi(argv[2]);
  std::cout << "using addr=" << addr << " port=" << port << std::endl;

  Server s(addr, port);
  s.run(); //loop forever, serving requests

  return 0;
}