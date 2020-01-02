#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;

#include "car.h"
#include "data.h"

Car::Car(string input)
{
  if(input.length() == 23){
  //TARGA_AUTO AA MM GG HH MM es. AB123CD 19 12 16 09 25 -> 22 caratteri
    plate_ = input.substr(0,7);
/*
    int MM = stoi(input.substr(11,2));
    int DD = stoi(input.substr(14,2));
    int HH = stoi(input.substr(17,2));
    int mm = stoi(input.substr(20,2));
*/
    Data d {input.substr(8,14)};
    date_time_=d;
    has_reservation_=false;
    valid_=true;
  }
  else if(input.length() == 41){//AA MM GG HH MM AB123CD AA MM GG AA MM GG-> 40 caratteri 19 09 21 22 11 3000001 19 10 29 19 12 09
    cout<<input.substr(15,7)<<endl;
    cout<<input.substr(0,14)<<endl;
    cout<<input.substr(23,8)<<endl;
    cout<<input.substr(32,8)<<endl;
    plate_ = input.substr(15,7);
    date_time_ = input.substr(0,14);
    res_enter_time_ = input.substr(23,8);
    res_exit_time_ = input.substr(32,8);
    has_reservation_=true;
    valid_=true;
  }
  else{
    valid_=false;
  }
}

Car::Car()
:valid_{false}{}


bool operator==(const Car& c1, const Car& c2)
{
  bool out;
  if(c1.valid() && c2.valid())
    out = (c1.plate() == c2.plate());
  else if(!(c1.valid()) && !(c2.valid()))
    out = true;
  else
    out = false;
  return out;
}

bool operator!=(const Car& c1, const Car& c2)
{
  return !(c1 == c2);
}

std::ostream& operator<<(std::ostream& os, const Car& c)
{
  if(c.valid()){
    os << "Plate: "<< c.plate()<<" "
    << "Date of entrance: " << c.date_time()<<endl;
  }
  else
    os <<"INVALID CAR"<<endl;
	return os;
}
