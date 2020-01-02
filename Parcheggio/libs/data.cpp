#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;
using std::to_string;

#include "data.h"

Data::Data(string data_string)
{
  if(data_string.length() == 14){
    year_ = stoi(data_string.substr(0,2));
    month_ = stoi(data_string.substr(3,2));
    day_ = stoi(data_string.substr(6,2));
    hour_ = stoi(data_string.substr(9,2));
    minute_ = stoi(data_string.substr(12,2));
    valid_ = true;
  }
  else if(data_string.length() == 8){
    year_ = stoi(data_string.substr(0,2));
    month_ = stoi(data_string.substr(3,2));
    day_ = stoi(data_string.substr(6,2));
    hour_ = 0;
    minute_ = 0;
    valid_ = true;
  }
  else{
    valid_ = false;
    year_ = 0;
    month_ = 0;
    day_ = 0;
    hour_ = 0;
    minute_ = 0;
  }

}

Data::Data()
:valid_{false}{}

//Calcola quanti minuti sono passati dal punto 0 considerato come 1/1/2000
long Data::get_min_in() const{
   long year = long(year_);
   long month = long(month_);
   long day = long(day_);
   long minutes = long(minute_);
   long hour = long(hour_);
   long min_out;
   min_out = ((year-1)/4)*2102400+((year-1)%4)*527040; //4*365*24*60 =2102400    366*24*60 =527040
   switch(month-1){
     case 11:min_out = min_out + 43200;  //30*60*24 =43200
            [[gnu::fallthrough]];
     case 10:min_out = min_out + 44640; //31*60*24 =44640
           [[gnu::fallthrough]];
     case 9:min_out = min_out + 43200;
           [[gnu::fallthrough]];
     case 8:min_out = min_out + 44640;
           [[gnu::fallthrough]];
     case 7:min_out = min_out + 44640;
           [[gnu::fallthrough]];
     case 6:min_out = min_out + 43200;
           [[gnu::fallthrough]];
     case 5:min_out = min_out + 44640;
           [[gnu::fallthrough]];
     case 4:min_out = min_out + 43200;
           [[gnu::fallthrough]];
     case 3:min_out = min_out + 44640;
           [[gnu::fallthrough]];
     case 2:if(year%4!=0)
               min_out = min_out + 29*24*60;
             else
               min_out = min_out + 28*24*60;
          [[gnu::fallthrough]];
     case 1:min_out = min_out + 44640;
            break;
   }
   min_out= min_out + day*1440; //24*60=1440
   min_out = min_out + hour*60;
   min_out = min_out + minutes;

  return min_out;
}

string Data::data_to_string() const
{
  string out = to_string(year_)+" ";
  out = out + to_string(month_)+" ";
  out = out + to_string(day_)+" ";
  out = out + to_string(hour_)+" ";
  out = out + to_string(minute_)+" ";
  return out;
}


bool operator> (const Data& d1, const Data& d2)
{
  bool out;
  if(d1.is_valid()&&d2.is_valid())
    out = (d1.get_min_in()>d2.get_min_in());
  else if(d1.is_valid() && !d2.is_valid())
    out = true;
  else if(!d1.is_valid() && d2.is_valid())
    out = false;
  else
    out = false;
  return out;
}

bool operator<(const Data& d1, const Data& d2)
{
  bool out;
  if(d1.is_valid()&&d2.is_valid())
    out = (d1.get_min_in()<d2.get_min_in());
  else if(d1.is_valid() && !d2.is_valid())
    out = true;
  else if(!d1.is_valid() && d2.is_valid())
    out = false;
  else
    out = false;
  return out;
}

bool operator==(const Data& d1, const Data& d2)
{
  return (d1.get_min_in()==d2.get_min_in());
}

bool operator!=(const Data& d1, const Data& d2)
{
  return (d1.get_min_in()!=d2.get_min_in());
}

std::ostream& operator<<(std::ostream& os, const Data& d)
{
	os << d.year() <<" "<<
  d.month()<< " "<<
  d.day()<< " "<<
  d.hour()<< " "<<
  d.minute();
	return os;
}
