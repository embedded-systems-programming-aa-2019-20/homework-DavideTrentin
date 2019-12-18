#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;

#include "car.h"

Car::Car(string input)
{
  //TARGA_AUTO AA MM GG HH MM es. AB123CD 19 12 16 09 25 -> 22 caratteri
    plate_ = input.substr(0,7);
/*
    int MM = stoi(input.substr(11,2));
    int DD = stoi(input.substr(14,2));
    int HH = stoi(input.substr(17,2));
    int mm = stoi(input.substr(20,2));
*/
    date_time_ = input.substr(8,14);
}


bool operator==(const Car& c1, const Car& c2)
{
  return (c1.plate() == c2.plate());
}

bool operator!=(const Car& c1, const Car& c2)
{
  return !(c1 == c2);
}

std::ostream& operator<<(std::ostream& os, const Car& c)
{
	os << "Plate: "<< c.plate()<< endl
  << "Date of entrance: " << c.date_time()<<endl;
	return os;
}
