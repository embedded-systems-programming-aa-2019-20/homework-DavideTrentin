#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;



#include "car.h"
int main() {

Car c1{"AB123CD 19 12 16 00 00"};
Car c2{"EF235GH 19 12 16 11 15"};
Car c3{"IL256MN 19 12 16 11 17"};
Car c4{"OP666QR 19 12 16 12 65"};
Car c5{"ST456UV 19 12 16 23 59"};

cout <<c1<<c2<<c3<<c4<<c5;

}
