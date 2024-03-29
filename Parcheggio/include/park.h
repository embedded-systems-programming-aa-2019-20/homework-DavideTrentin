// Park.h

#ifndef PARK_H
#define PARK_H

#include <mutex>
#include <list>
#include <condition_variable>
#include <map>


#include "car.h"
#include "data.h"

class Park {
public:
    Park(string park_name, int CAP, float hourly_rate, float dayly_rate);

    void car_enters(const Car& c);
    void car_exits(const Car& c);
    string park_status();

    //Funzione per thread calcolo profitti
    void calc_profit();
    void car_exits_output(const Car& c);

    int CAP() const {return CAP_;}
    int occ() const {return occ_;}
    float profit() const {return profit_;}
    string park_name() const {return park_name_;}
    std::list<Car> car_in_park() const {return buffer_;}
    int cars_wto_elaborate();

private:
    std::mutex mutex_;

    std::list<Car> buffer_;
    int CAP_; //CAPACITY
    int occ_; //occupancy
    string park_name_;

//VARIABILI PER IL CALCOLO DEI GUADAGNI
    float profit_;
    float old_profit_;
    std::condition_variable calculated_;
    bool b_calculated_;
    const float hourly_rate_;
    const float dayly_rate_;
    std::list<Car> enters_wto_elaborate_;
    std::list<Car> exits_wto_elaborate_;
    int cars_wto_elaborate_;

};

#endif
