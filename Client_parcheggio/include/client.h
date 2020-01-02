#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <string>
using std::string;

class Client{
public:
  Client(const char* IP,const char* PORT);
  void client_close_conn();
  string client_receive();
  int client_open_conn();
  void client_send(string to_send);

private:
  int sockfd, numbytes;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  const char* IP_;
  const char* PORT_;
};

#endif
