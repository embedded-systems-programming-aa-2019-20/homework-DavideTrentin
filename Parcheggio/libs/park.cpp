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
#include<map>

#include "car.h"
#include "park.h"
#include "data.h"

//COSTRUTTORI=============================================================================================
Park::Park(string park_name, int cap, float hourly_rate, float dayly_rate)
: CAP_(cap),park_name_(park_name),profit_(0), hourly_rate_(hourly_rate), dayly_rate_(dayly_rate),
cars_wto_elaborate_(0){
cout << "parcheggio: "<< park_name_ << " posti disponibili: "<< to_string(CAP_)<< endl;
}

//FUNZIONI===================================================================================================




//FUNZIONI MEMBRO=========================================================================
void Park::car_enters(const Car& c)
{
  unique_lock<std::mutex> mlock(mutex_);
  if(occ_ < CAP_ ||c.has_reservation()){
    if(c.has_reservation()){
      buffer_.push_back(c);
      ++occ_;
    }
    else{
      buffer_.push_back(c);
      ++occ_;
    }

    cout << c.date_time() << " " << c.plate() << " ENTERS " << park_name()<< endl;
    }
  else{
    cerr << "l'auto " << c.plate()<< " ha trovato il parcheggio pieno"<<endl;
  }
}

int Park::cars_wto_elaborate()
{
  unique_lock<std::mutex> mlock(mutex_);
  return cars_wto_elaborate_;
}

void Park::car_exits(const Car& c)
{
  std::list<Car>::iterator it;
  unique_lock<std::mutex> mlock(mutex_);
  it = find(buffer_.begin(), buffer_.end(), c);
  if(it != buffer_.end()){
    enters_wto_elaborate_.push_back((*it));
    exits_wto_elaborate_.push_back(c);
    ++cars_wto_elaborate_;
    cout << c.date_time() << " " << (*it).plate() << " EXITS " << park_name()<< endl;
    buffer_.erase(it);
    --occ_;
  }
  else{
    cerr << "Park error: auto "<< c.plate() <<" non presente"<< endl;
    enters_wto_elaborate_.push_back(Car{"0"});
    exits_wto_elaborate_.push_back(Car{"0"});
    ++cars_wto_elaborate_;
  }
}

string Park::park_status()
{
  std::list<Car>::iterator it;
  string out = "Posti occupati in " + park_name()+ " : " + to_string(occ_)
                + " Ricavi: " + to_string(profit_) + " auto presenti: \n";
  for(it = buffer_.begin(); it != buffer_.end(); ++it){
    if((*it).has_reservation())
      out = out + (*it).plate() +" "+ (*it).res_enter_time().data_to_string() +"\n";
    else
      out = out + (*it).plate() +" "+ (*it).date_time().data_to_string() +"\n";
  }
  return out;
}

void Park::calc_profit()
{
  unique_lock<std::mutex> mlock(mutex_);
  if(cars_wto_elaborate_>0){
    Car c1 = *(exits_wto_elaborate_.begin());
    exits_wto_elaborate_.pop_front();
    Car c2 = *(enters_wto_elaborate_.begin());
    enters_wto_elaborate_.pop_front();
    --cars_wto_elaborate_;
    mlock.unlock();
    float temp, days_due, hours_due;
    if(c1.valid()&&c2.valid()){
      if(!c1.has_reservation()){
        temp = c1.date_time().get_min_in() - c2.date_time().get_min_in();
        days_due = float(int(temp/float(60*24)) * dayly_rate_);
        hours_due = float((int(temp) % 60*24)/60) * hourly_rate_;
      }
      else{
        temp = c1.res_exit_time().get_min_in() - c2.res_enter_time().get_min_in();
        days_due = float(int(temp/float(60*24)) * dayly_rate_);
        hours_due = float((int(temp) % 60*24)/60) * hourly_rate_;
      }
      if(days_due>0 && hours_due>0)
      {
        hours_due = 0;
        days_due = days_due + dayly_rate_;
      }

      mlock.lock();
      profit_ = profit_ + hours_due + days_due;
      mlock.unlock();
  }
  }
}
