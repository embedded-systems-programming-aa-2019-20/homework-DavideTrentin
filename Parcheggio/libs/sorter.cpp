#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include "data.h"
#include "car.h"
#include "sorter.h"
Sorter::Sorter(Car c, int park_id, bool enters)
:car_(c),park_id_(park_id),enters_(enters),has_res_(false)
{

}

Sorter::Sorter(Car c, bool has_res)
:car_(c),has_res_(has_res)
{

}
