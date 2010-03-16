// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 03/15/2010 19:00:21
//           By: Luo Qiang
//     Update #: 10
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include<itpp/itbase.h>
#include"regular.h"
#include<iostream>
#include<fstream>
using		std::cout;
using		std::cin;
using		std::endl;
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
  Sparse_Mat<int>	matrix = regular(n,d);
  timer.toc_print();
  out<<matrix;
  return 0;
}

// 
// main.cpp ends here
