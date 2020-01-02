
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
extern condition_variable car_went_out[2];

extern bool done[11]; //segnalatore end thread

extern multimap<Data,Sorter> next_map;
extern condition_variable map_filled;

extern Park p1;
extern Park p2;

extern char* PORT;
extern const int MAXDATASIZE;
extern bool stop;
extern Server serv;

#endif
