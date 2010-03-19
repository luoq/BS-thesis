// A test
// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 03/19/2010 10:01:12
//           By: Luo Qiang
//     Update #: 31
// Keywords: 

// Commentary: 

// Change log:

// Code:
//#include<itpp/itbase.h>
#include <boost/timer.hpp>
#include"regular.h"
#include<iostream>
#include<fstream>
using	std::cout;
using	std::cin;
using	std::endl;
//using		itpp::Real_Timer;
using boost::timer;
int main(){
  srand(time(NULL));
  int		n,d;
  timer	timer;
  ofstream	out("data");
  smat<int>	matrix;
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  cout<<"input n and d:"<<endl;
  cin>>n>>d;
  timer.restart();
  matrix = regular(n,d);
  cout<<"Time elapsed: "<<timer.elapsed()<<endl;
  out<<matrix;
  out.close();
  return 0;
}

// 
// main.cpp ends here
