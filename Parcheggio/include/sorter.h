#ifndef SORTER_H
#define SORTER_H

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include "data.h"
#include "car.h"

class Sorter {
public:
  Sorter(Car c, int park_id, bool enters);
  Sorter(Car c, bool has_res);
  Car car()const{return car_;}
  int park_id() const{return park_id_;}
  bool enters() const{return enters_;}
  bool has_reservation()  const{return has_res_;}
private:
  Car car_;
  int park_id_;
  bool enters_;
  bool has_res_;

};

#endif
