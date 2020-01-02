
#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <mutex>
#include <string>
using std::string;
using std::stoi;
#include <condition_variable>
#include <mutex>
using std::mutex;
using std::unique_lock;
using std::condition_variable;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
using std::multimap;

extern mutex rw_mutex;
extern condition_variable done_reading;
extern condition_variable car_went_out[2];
extern condition_variable res_inserted;

extern bool done[11]; //segnalatore end thread

extern multimap<Data,Sorter> next_map;
extern condition_variable map_filled;
extern list<Sorter> res_to_insert;

extern Park p1;
extern Park p2;

extern const char* PORT;
extern const int MAXDATASIZE;
extern bool stop;
extern Server serv;

#endif
