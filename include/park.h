// Park.h

#ifndef PARK_H
#define PARK_H

#include <mutex>
#include <list>
#include "car.h"
class Park {
public:
    Park(string park_name, int CAP);
    
    void car_enters(const Car& c);
    void car_exits(const Car& c);
    string park_status();

    int CAP() const {return CAP_;}
    int occ() const {return occ_;}
    string park_name() const {return park_name_;}
    std::list<Car> car_in_park() const {return buffer_;}


private:
    std::list<Car> buffer_;
    std::mutex mutex_;
    int CAP_; //CAPACITY
    int occ_; //occupancy
    string park_name_;


};

#endif
