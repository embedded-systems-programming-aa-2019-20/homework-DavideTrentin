/*
** server.c -- a stream socket server demo
*/


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

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <thread>
using std::thread;


#include <algorithm>
using std::find;

#include "server.h"

void Server::make_accept_sock (const char* PORT) {

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC; // PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE; // use my IP

    struct addrinfo* servinfo;
    int rv = getaddrinfo("localhost", PORT, &hints, &servinfo);
    if (rv != 0) {
        cerr << "getaddrinfo:" << gai_strerror(rv) << endl;
        exit(EXIT_FAILURE);
    }

    struct addrinfo  *p;
    int sockfd; int yes{1};
    // loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {

        // create a socket
        // socket(int domain, int type, int protocol)
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			cerr << "server: failed to create socket\n";
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			cerr << "server: failed setsockopt\n";
			exit(EXIT_FAILURE);
		}

        // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
        // bind() passes file descriptor, the address structure,
        // and the length of the address structure
        // This bind() call will bind the socket to the current IP address
        // on port
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			cerr << "server: failed bind\n";
			continue;
		}

		break;
	}

	if (p == NULL)  {
		cerr << "server: failed to bind\n";
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo); // all done with this structure

    // This listen() call tells the socekt to listen to the incoming connections.
    // The listen() function places all incoming connection into a backlog queue
    // until accept() call accepts the connection
    // Here we set the maximum size for the backlog queue to 10.

    const int BACKLOG{10};	 // how many pending connections queue will hold
	if (listen(sockfd, BACKLOG) == -1) {
		cerr << "server: failed to listen\n";
	    exit(EXIT_FAILURE);
	}

    accept_sock_ =  sockfd;

}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




Server::Server(const char* PORT)
:PORT_{PORT}, accept_sock_{-1} {}

Server::Server()
:PORT_{0000}, accept_sock_{-1} {}



void Server::server_send(int com_sock, string to_send)
{
  to_send = to_send +"\n";
	if (send(com_sock, to_send.c_str(), to_send.length(), 0) == -1)
	cerr << "send  \n";
}

string Server::server_receive(int com_sock)
{
	char buf[100];
  string out;
  int numbytes;
  memset(buf, 0, 100-1);
  if ((numbytes = recv(com_sock, buf, 100-1, 0)) == -1) {
    cerr << "Client failed to receive data\n";
    exit(EXIT_FAILURE);
  }
  buf[numbytes] = '\0';

  for(int i=0; i<100;i++){
    out = out + buf[i];
  }
  //cout << "server received: "<< out << endl;
  return out;
}

void Server::server_close_com(int com_sock)
{
    close(com_sock);
}

void Server::server_close_accept()
{
    close(accept_sock_);
    accept_sock_ = -1;
}
