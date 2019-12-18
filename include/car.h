// Car.h

#ifndef CAR_H
#define CAR_H

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;



class Car {
public:
  Car(string input);
  string plate() const { return plate_; }
  string date_time() const { return date_time_; }

private:
  string plate_;
  string date_time_;
//  string exit_date_time_;


};

std::ostream& operator<<(std::ostream& os, const Car& c);
bool operator==(const Car& c1, const Car& c2);
bool operator!=(const Car& c1, const Car& c2);
#endif
