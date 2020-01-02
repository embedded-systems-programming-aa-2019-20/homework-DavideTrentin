#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
using std::string;


class Server{
public:
  Server(const char* PORT);
  Server();
  void server_start();
  void server_send (int com_sock, string to_send);
  string server_receive(int com_sock);
  void server_close_com(int com_sock);
  void server_close_accept();
  void make_accept_sock (const char* PORT);

  void mod_port(const char* PORT){PORT_ = PORT;}
  int accept_sock()const {return accept_sock_;}

private:
  const char* PORT_;
  int accept_sock_;
};
#endif
