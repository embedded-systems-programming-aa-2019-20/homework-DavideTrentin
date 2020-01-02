#ifndef DATA_H
#define DATA_H

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;

class Data {
public:
  Data(string data_string); //"YY MM DD HH mm"
  Data();

  long get_min_in()const;

  int year() const { return year_; }
  int month() const { return month_; }
  int day() const { return day_; }
  int hour() const { return hour_; }
  int minute() const { return minute_; }
  bool is_valid() const {return valid_;}
  string data_to_string() const;
private:
  int year_;
  int month_;
  int day_;
  int hour_;
  int minute_;
  bool valid_;



};

std::ostream& operator<<(std::ostream& os, const Data& d);
bool operator<(const Data& d1, const Data& d2);
bool operator>(const Data& d1, const Data& d2);
bool operator==(const Data& d1, const Data& d2);
bool operator!=(const Data& d1, const Data& d2);







#endif
