// park.cpp
#include <mutex>
using std::unique_lock;
#include <list>
using std::list;
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;
using std::to_string;
#include <algorithm>
using std::find;

#include "car.h"
#include "park.h"

Park::Park(string park_name, int cap)
: CAP_(cap),park_name_(park_name){
cout << "parcheggio: "<< park_name_ << " posti disponibili: "<< to_string(CAP_)<< endl;
}


void Park::car_enters(const Car& c)
{
  unique_lock<std::mutex> mlock(mutex_);
  if(occ_ < CAP_){
    buffer_.push_back(c);
    ++occ_;
    cout << c.date_time() << " " << c.plate() << " ENTERS " << park_name()<< endl;
    }
  else{
    cerr << "l'auto " << c.plate()<< " ha trovato il parcheggio pieno"<<endl;
  }
}


void Park::car_exits(const Car& c)
{
  std::list<Car>::iterator it;
  unique_lock<std::mutex> mlock(mutex_);
  it = find(buffer_.begin(), buffer_.end(), c);
  if(it != buffer_.end()){
    cout << c.date_time() << " " << (*it).plate() << " EXITS " << park_name()<< endl;
    buffer_.erase(it);
    --occ_;
  }
  else{
    cerr << "Park error: auto non presente" << endl;
  }
}

string Park::park_status()
{
  std::list<Car>::iterator it;
  string out = "posti occupati:" + to_string(occ_) + " auto presenti: \n";
  for(it = buffer_.begin(); it != buffer_.end(); ++it){
    out = out + (*it).plate() +" "+ (*it).date_time() +"\n";
  }
  return out;
}
