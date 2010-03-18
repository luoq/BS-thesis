// A test
// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 03/17/2010 22:52:58
//           By: Luo Qiang
//     Update #: 14
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include<itpp/itbase.h>
#include"regular.h"
#include<iostream>
#include<fstream>
using	std::cout;
using	std::cin;
using	std::endl;
using itpp::Real_Timer;
int main(){
  srand(time(NULL));
  int		n,d;
  Real_Timer	timer;
  ofstream	out("data");
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  cout<<"input n and d:"<<endl;
  cin>>n>>d;
  timer.tic();
  smat<int>	matrix = regular(n,d);
  timer.toc_print();
  out<<matrix;
  out.close();
  return 0;
}

// 
// main.cpp ends here
