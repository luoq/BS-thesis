// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 03/20/2010 10:41:49
//           By: Luo Qiang
//     Update #: 55
// Keywords: 

// Commentary: 

// Change log:

// Code:
//#include<itpp/itbase.h>
//using	itpp::Real_Timer;
//typedef Real_Timer Timer
#include "Timer.h"
#include "regular.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main(){
  srand(time(NULL));
  int		n,d,trytimes;
  Timer	timer;
  double time;
  ofstream	out("matrix.data");
  smat<int>	matrix;
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  cout<<"input n and d:"<<endl;
  cin>>n>>d;
  timer.tic();
  matrix = regular(n,d,trytimes);
  time   = timer.toc();
  cout<<"tryed "<<trytimes<<" times\n";
  cout<<"Time elapsed: "<<time<<" ms\n";
  out<<matrix;
  if(!isregular(matrix,d))
  {
	  return 1;
  }
  out.close();
  return 0;
}

// 
// main.cpp ends here
