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

#include "car.h"
#include "park.h"

//VARIABILI GLOBALI========================================================================================================
string next_line[4]{"0","0","0","0"}; //[entrate1,uscite1,entrate2,uscite2]
mutex rw_mutex;
condition_variable next_line_empty[4];
condition_variable new_line_ready;
bool new_line_ready_bool{false};
int done = 0;

Park p1("PARK1",1000);
Park p2("PARK2",1000);

//FUNZIONI =====================================================================================================================
bool date_is_first(string line1, string line2){
  bool out{false};
  if(line1.compare ("0")!=0 && line2.compare ("0")!=0){
    int AA1;
    int AA2;
    for(int i = 8; i <= 20; i+=3){
      AA1 = stoi(line1.substr(i,2));
      AA2 = stoi(line2.substr(i,2));
      if(AA1<AA2){
        out = true;
        break;
      }
      else if(AA1>AA2){
        out = false;
        break;
      }
    }
  }
  else if(line1.compare ("0")==0)
    out = false;
  else
    out = true;

  return out;
}

bool next_line_control(int index)
{
  return next_line[index].compare ("0")==0;
}

void print_state(){
  cout<<endl<<"ARRAY next_line"<<endl<<
  next_line[0]<<endl<<
  next_line[1]<<endl<<
  next_line[2]<<endl<<
  next_line[3]<<endl<<endl;
}
//FUNZIONI THREAD=============================================================================================================

void th_file_reader(string file_name, int index)
{

  string line{"0"};
  ifstream myfile (file_name);
  if (myfile.is_open())
    {
      while ( std::getline (myfile,line)){
        unique_lock<mutex> rw_lock(rw_mutex);

        while(next_line[index].compare ("0")!=0)
          next_line_empty[index].wait(rw_lock);

        next_line[index] = line;
        new_line_ready_bool = true;
        new_line_ready.notify_one();
        cout<<"aggiunta: "<<next_line[index]<<endl;
        rw_lock.unlock();
      }
    myfile.close();
    next_line[index] = "0";
    ++done;
    }
  else cout << "errore lettura file"<<endl;

}


void th_chooser()
{
  string next = next_line[0];
  int index;
  int gate_id;
  while(done < 4){
    unique_lock<mutex> rw_lock(rw_mutex);
    while(!new_line_ready_bool)
      new_line_ready.wait(rw_lock);

    next = next_line[gate_id];
    print_state();
    for(int index = 0; index < 4; index++){
        if(date_is_first(next_line[index],next)){
          next = next_line[index];
          gate_id = index;
        }
      }
    next_line[gate_id] = "0";
    new_line_ready_bool = false;
    next_line_empty[gate_id].notify_one();
    rw_lock.unlock();
    cout<<"the next is: "<<next<<endl;


    if(done ==1 ){
      cerr << p1.park_status();
      }

    


    switch(gate_id){
      case 0: p1.car_enters(Car{next});
              break;
      case 1: p1.car_exits(Car{next});
              break;
      case 2: p2.car_enters(Car{next});
              break;
      case 3: p2.car_exits(Car{next});
              break;
      default: break;
    }

    }
  }




//MAIN======================================================================================================================
int main() {

Car c1{"AB123CD 19 12 16 00 00"};
Car c2{"EF235GH 19 12 16 11 15"};
Car c3{"IL256MN 19 12 16 11 17"};
Car c4{"OP666QR 19 12 16 12 65"};
Car c5{"ST456UV 19 12 16 23 59"};
cout <<c1<<c2<<c3<<c4<<c5<<endl;

cout<< date_is_first("AB123CD 19 12 19 00 12","EF235GH 19 12 17 11 15")<<endl;


vector<thread> threads;

threads.push_back(thread{th_file_reader,"src/parcheggioUnoIngresso.txt", 0});
threads.push_back(thread{th_file_reader,"src/parcheggioUnoUscita.txt", 1});
threads.push_back(thread{th_file_reader,"src/parcheggioDueIngresso.txt", 2});
threads.push_back(thread{th_file_reader,"src/parcheggioDueUscita.txt", 3});

std::this_thread::sleep_for(std::chrono::milliseconds(1000));
threads.push_back(thread{th_chooser});


cout << "Main thread: wait thread to end\n";

vector<thread>::iterator threads_it;
for(threads_it = threads.begin(); threads_it != threads.end(); ++threads_it){
  (*threads_it).join();
}


  return 0;



}
