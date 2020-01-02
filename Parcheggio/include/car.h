// Car.h

#ifndef CAR_H
#define CAR_H

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include "data.h"


class Car {
public:
  Car(string input);
  Car();

  string plate() const { return plate_; }
  Data date_time() const { return date_time_; }
  bool valid() const{return valid_;}
  Data res_enter_time() const { return res_enter_time_; }
  Data res_exit_time() const { return res_exit_time_; }
  bool has_reservation() const {return has_reservation_;}
private:
  string plate_;
  Data date_time_;
  Data res_enter_time_;
  Data res_exit_time_;
  bool has_reservation_;
  bool valid_;
//  string exit_date_time_;


};

std::ostream& operator<<(std::ostream& os, const Car& c);
bool operator==(const Car& c1, const Car& c2);
bool operator!=(const Car& c1, const Car& c2);
#endif
