#include <mutex>
#include <list>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;
#include <thread>
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
//FUNZIONI THREAD=============================================================================================================

// Funzione per la thread di lettura in input ci sono name file e indice della thread.
// Ogni thread elabora la stringa che legge definendo l'oggetto Car associato
// e inserendolo in un oggetto Sorter che serve a definire su quale percheggio agisce
// e se entra/esce. Il Sorter viene aggiunto alla MULTIMAP un contenitore ordinato
// rispetto alla variabile chiave "car.date_time()" cioè data dell'esecuzione dell'azione
void th_file_reader(string file_name, int index) //indice 0 1 2 3 4
{
  string line{"0"};
  ifstream myfile (file_name);
  if (myfile.is_open())
    {
      while ( std::getline (myfile,line)){
        unique_lock<mutex> rw_lock(rw_mutex);
        Car c {line};
        switch(index){
          case 0: next_map.insert(std::make_pair(c.date_time(), Sorter {c,0,true}));
                  break;
          case 1: next_map.insert(std::make_pair(c.date_time(), Sorter {c,0,false}));
                  break;
          case 2: next_map.insert(std::make_pair(c.date_time(), Sorter {c,1,true}));
                  break;
          case 3: next_map.insert(std::make_pair(c.date_time(), Sorter {c,1,false}));
                  break;
          case 4: next_map.insert(std::make_pair(c.date_time(), Sorter {c,1}));
                  break;
        }
        cout<<"aggiunta: "<<c<<endl;
        rw_lock.unlock();
      }
      myfile.close();
      unique_lock<mutex> rw_lock(rw_mutex);
      cerr<<"DONE: "<<index<<endl;
      done[index] = true;
      ++last;
      if(last == 5){
      rw_lock.unlock();
      map_filled.notify_one();
      }
    }
  else {
    cerr << "ERRORE lettura file: "<<file_name<<endl;
    exit(EXIT_FAILURE);
  }

}

/*
void th_res_reader(string file_name) //indice 4
{
  int index = 4;
  string line{"0"};
  ifstream myfile (file_name);
  if (myfile.is_open()){
    while ( std::getline (myfile,line)){
      unique_lock<mutex> rw_lock(rw_mutex);
      cout<<"sono la thread "<< index<<endl;
      Car c {line};
      next_map.insert(std::make_pair(c.date_time(), Sorter {c,1}));
      cout<<"aggiunta 4 prenot: "<<c<<endl;
      rw_lock.unlock();
    }
    myfile.close();
    unique_lock<mutex> rw_lock(rw_mutex);
    cerr<<"DONE: "<<index<<endl;
    done[index] = true;
    rw_lock.unlock();
    map_filled.notify_one();
  }
  else{
    cerr << "ERRORE lettura file: "<<file_name<<endl;
    exit(EXIT_FAILURE);
  }
}*/

// Funzione per la thread di esecuzione delle azioni.
// il ciclo continua finchè non è richiesto lo stop dal client e se ci sono
// ancora dati da eseguire. La condizione di blocco sul wait è data dalla fine
// della lettura dei file in ingresso. Successivamente vengono elavorati ad uno
// ad uno gli elementi nella multimap dividendo i casi di Prenotazioni e Uscite/Entrate
// PRENOTAZIONI: inserisco nella map la prenotazione nel parcheggio più libero al
// momento della prenotazione.
// ENTRATE: viene eseguita l'entrata nel parcheggio indicato dall'oggetto Sorter.
// USCITE: viene eseguita l'uscita nel parcheggio  indicato dall'oggetto Sorter e
// viene segnalato alla relativa thread di calcolo dei profitti che c'è un nuovo dato
// pronto per essere calcolato
void th_executer()// indice 8
{
  int index = 8;
  unique_lock<mutex> rw_lock(rw_mutex);
  while(next_map.size()>0 && (!stop)){
    while(!done[0]||!done[1]||!done[2]||!done[3]||!done[4])
      map_filled.wait(rw_lock);
    multimap<Data,Sorter>::iterator it = next_map.begin();

    if(next_map.size()>0){

      if((*it).second.has_reservation()){
        if((*it).second.car().valid()){
          if((p1.CAP()-p1.occ())<(p1.CAP()-p2.occ())){
            Car c1 = (*it).second.car();
            next_map.insert(std::make_pair(c1.res_enter_time(), Sorter {c1,0,true}));
            next_map.insert(std::make_pair(c1.res_exit_time(), Sorter {c1,0,false}));
            cout<<c1.date_time()<< " " <<c1.plate()<<" ENLISTED FOR PARK 1"<<endl;
          }
          else{
            Car c1 = (*it).second.car();
            next_map.insert(std::make_pair(c1.res_enter_time(), Sorter {c1,1,true}));
            next_map.insert(std::make_pair(c1.res_exit_time(), Sorter {c1,1,false}));
            cout<<c1.date_time()<< " " <<c1.plate()<<" ENLISTED FOR PARK 2"<<endl;
          }
        }
        next_map.erase(it);
      }
      else{
        if((*it).second.park_id()==0){
          if((*it).second.enters())
            p1.car_enters((*it).second.car());
          else{
            p1.car_exits((*it).second.car());
            rw_lock.unlock();
            car_went_out[0].notify_one();
            rw_lock.lock();
          }
        }
        if((*it).second.park_id()==1){
          if((*it).second.enters())
            p2.car_enters((*it).second.car());
          else{
            p2.car_exits((*it).second.car());
            rw_lock.unlock();
            car_went_out[1].notify_one();
            rw_lock.lock();
          }
        }
        next_map.erase(it);
      }
    }
  rw_lock.unlock();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  rw_lock.lock();
  if(next_map.size()==0)
    stop = true;
  }

  cerr<<"DONE: "<<index<<endl;
  done[index] = true;
  rw_lock.unlock();
  car_went_out[0].notify_one();
  car_went_out[1].notify_one();
}

// Funzione per la thread di calcolo dei profitti.
// input il numero del parcheggio (1 o 2). La condizione del wait aspetta di ricevere
// nuovo dati da calcolare (derivanti dall'uscita di una macchina). Richiama la
// funzione calcolo dei profitti del parcheggio.
void th_profit_calc(int parkid) //indice 5 6
{
  unique_lock<mutex> rw_lock(rw_mutex);
  if(parkid == 0){
    while(!done[8] || p1.cars_wto_elaborate()>0){
      while(p1.cars_wto_elaborate()==0 && !done[8])
        car_went_out[parkid].wait(rw_lock);
      rw_lock.unlock();
      p1.calc_profit();
      rw_lock.lock();
      cout<< "profit PARK1: "<<p1.profit()<<endl;
    }
    done[5]=true;
    cerr<< "DONE 5"<<endl;
  }
  if(parkid == 1){
    while(!done[8] || p2.cars_wto_elaborate()>0){
      while(p2.cars_wto_elaborate()==0&& !done[8])
        car_went_out[parkid].wait(rw_lock);
      rw_lock.unlock();
      p2.calc_profit();
      rw_lock.lock();
      cout<< "profit PARK2: "<<p2.profit()<<endl;
    }
    done[6]=true;
    cerr<< "DONE 6"<<endl;
  }
}

// Funzione per la thread di comunicazione
// Dopo essere stata creata aspetta di ricevere le richieste ed invia la risposta
// seguita da un "DONE****" che rappresenta la fine della comunicazione
void th_socket(int com_sock)//indice 10
{

    string rec = serv.server_receive(com_sock);
    string to_send;
    if(stoi(rec)>=0 && stoi(rec) <= 2){
      switch(stoi(rec)){
        case 0: to_send = std::to_string(p1.profit()+p2.profit()); //ricavo totale
                break;
        case 1: to_send = p1.park_status() + "\n" + p2.park_status();
                break;
        case 2: unique_lock<mutex> rw_lock(rw_mutex);
                stop = true;
                rw_lock.unlock();
                break;
      }
      vector<string>::iterator it;
      vector<string> rdy_send;
      std::string::size_type pos = 0;
      std::string::size_type prev = 0;
      string delimiter = "\n";
      while ((pos = to_send.find(delimiter, prev)) != std::string::npos)
      {
          rdy_send.push_back(to_send.substr(prev, pos - prev));
          prev = pos + 1;
      }
      rdy_send.push_back(to_send.substr(prev));
      for(it = rdy_send.begin(); it < rdy_send.end(); it++ ){
        serv.server_send(com_sock,(*it));
      }
      serv.server_send(com_sock,"DONE****");
    }
  serv.server_close_com(com_sock);
  done[10] = true;
  cerr<<"DONE SENDING DATA"<<endl;
}

// Funzione loop accettazione comunicazione
// crea il canale di comunicazione in una thread separata.
void th_socket_accepter() //indice 9
{
  serv.make_accept_sock(PORT);
  while(!done[5]||!done[6]){

      int com_sock = accept(serv.accept_sock(), 0, 0);
      if (com_sock < 0) {
          cerr << "server: failed on accept\n";
          exit(EXIT_FAILURE);
      }
      thread t{th_socket, com_sock};
      t.detach();
  }
  serv.server_close_accept();
}
