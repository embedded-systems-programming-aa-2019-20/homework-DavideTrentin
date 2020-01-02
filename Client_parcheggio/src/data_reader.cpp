#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdexcept>      // std::invalid_argument
#include <bitset>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
using std::stoi;

#include "client.h"

int main()
{
  const char* PORT{"3500"};
  const char* IP{"localhost"};
  Client Connection{IP,PORT};
  bool status = Connection.client_open_conn();
  while(status){

    cout <<"Benvenuto al gestore di parcheggi"<<endl
        <<"Premi il tasto corrispondente ed INVIO per avviare un'opzione: " <<endl
        << "0 : per ottenere dati sui Profitti"<< endl
        << "1 : per ottenere la lista delle macchine in ogni parcheggio"<< endl
        << "2 : per terminare il sistema parcheggio"<<endl
        << "3 : per terminare solo il client"<<endl;
    string input;
    cin>>input;
    try {
      if(stoi(input)==0||stoi(input)==1){
        Connection.client_send(input);
        string rec;
        while((int)rec.find("DONE****") == -1){
          rec =  rec + Connection.client_receive();
        }
        rec.erase(rec.find("DONE****"),rec.length()-1);
        cout<<rec<<endl;
      }
      else if(stoi(input) == 3||stoi(input)==2){
        Connection.client_send(input);
        status = false;
      }
      else
        Connection.client_send("9");
    }
    catch (const std::invalid_argument& ia) {
	    cerr << "Invalid argument: " << ia.what() << '\n';
      Connection.client_send("9");
    }
    cout << "DONE"<<endl;
    Connection.client_close_conn();
    if(status)
      status = Connection.client_open_conn();
    }
  cerr<<"CHIUSURA CONNESSIONE"<<endl;
return 0;
}
