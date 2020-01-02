#include <mutex>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;
#include <fstream>
using std::ifstream;
#include <thread>
using std::thread;
#include <condition_variable>
#include <mutex>
using std::mutex;
using std::unique_lock;
using std::condition_variable;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
using std::multimap;

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

#include "data.h"
#include "car.h"
#include "park.h"
#include "sorter.h"
#include "server.h"
#include "main_functions.h"
#include "main_global_var.h"
//VARIABILI GLOBALI========================================================================================================
 mutex rw_mutex;
 condition_variable car_went_out[2];

 bool done[11]{false}; //segnalatore end thread

 multimap<Data,Sorter> next_map;
 condition_variable map_filled;

 Park p1("PARK1", 500, 1, 0.5);
 Park p2("PARK2", 500, 1, 0.5);

 char* PORT;
 const int MAXDATASIZE = 100;
 bool stop = false;
 Server serv;


//MAIN======================================================================================================================
int main(int argc, char* argv[])
{
  if (argc != 2) {
      cerr << "eseguire con: ./main PORT\n";
      exit(EXIT_FAILURE);
  }
  PORT = argv[1];
  serv.mod_port(PORT);

vector<thread> threads;

threads.push_back(thread{th_file_reader,"parcheggioUnoIngresso.txt", 0});
threads.push_back(thread{th_file_reader,"parcheggioUnoUscita.txt", 1});
threads.push_back(thread{th_file_reader,"parcheggioDueIngresso.txt", 2});
threads.push_back(thread{th_file_reader,"parcheggioDueUscita.txt", 3});

threads.push_back(thread{th_file_reader,"prenotazioni.txt",4}); //4

threads.push_back(thread{th_profit_calc,0});//5
threads.push_back(thread{th_profit_calc,1});//6


threads.push_back(thread{th_executer}); //8

threads.push_back(thread{th_socket_accepter}); //9


cout << "Main thread: wait thread to end\n";

vector<thread>::iterator threads_it;
for(threads_it = threads.begin(); threads_it != threads.end(); ++threads_it){
  (*threads_it).join();
}

cout << "Main thread: End Of Program\n";
  return 0;



}
