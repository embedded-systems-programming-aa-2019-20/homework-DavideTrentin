#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
using std::string;

void th_file_reader(string file_name, int index) ;//indice 0 1 2 3
void th_res_reader(string file_name); //indice 4
void th_chooser();//indice 7
void th_executer();// indice 8
void th_profit_calc(int parkid); //indice 5 6
void th_socket(int com_sock);//indice 10
void th_socket_accepter(); //indice 9


#endif
