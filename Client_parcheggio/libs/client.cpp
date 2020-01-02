/*
** client.cpp -- a stream socket client demo
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include "client.h"
//const char* PORT{"3490"}; // the port client will be connecting to

// max number of bytes we can get at once
Client::Client(const char* IP,const char* PORT)
:IP_{IP}, PORT_{PORT}{

}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int Client::client_open_conn()
{
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(IP_, PORT_, &hints, &servinfo)) != 0) {
		cerr <<  "getaddrinfo: " << gai_strerror(rv) << endl;
    return false;
		exit(EXIT_FAILURE);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			cerr << "Failed to create socket:\n";
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			cerr << "Failed to connect to socket...";
			continue;
		}

		break;
	}

	if (p == NULL) {
		cerr <<  "client: failed to connect\n";
    return false;
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof(s));
	cout << "client: connecting to " << s << endl;

	freeaddrinfo(servinfo); // all done with this structure
  return sockfd;
}


void Client::client_close_conn()
{
	close(sockfd);
	cerr<< "CLIENT: CONNECTION CLOSED"<<endl;
}

string Client::client_receive()
{
	char buf[100];
  string out;
	memset(buf, 0, 100-1);
  if ((numbytes = recv(sockfd, buf, 100-1, 0)) == -1) {
    cerr << "Client failed to receive data\n";
    exit(EXIT_FAILURE);
  }
  buf[numbytes] = '\0';

  for(int i=0; i<100;i++){
    out = out + buf[i];
  }
  //cout << "client received: "<< out << endl;
  return out;
}

void Client::client_send(string to_send)
{
	if (send(sockfd, to_send.c_str(), to_send.length(), 0) == -1)
	cerr << "send  \n";
}
