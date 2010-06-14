// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 06/14/2010 09:03:50
//           By: Luo Qiang
//     Update #: 74
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
int main(int argc,char** argv){
  int	n,d;
  Timer	timer;
  double time;
  smat<int> matrix;

  if(argc!=4)
    {
      cout<<"Usage: "<<argv[0]<<" n d pathtosave\n";
      return 1;
    }
  ofstream	out(argv[3]);
  n=atoi(argv[1]);
  d=atoi(argv[2]);
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  
  seed_rand();
  timer.tic();
  matrix = regular(n,d);
  time   = timer.toc();
  cout<<"Time elapsed: "<<time<<" ms\n";
  out<<matrix;
  if(!isregular(matrix,d))
  {
    cout<<"regular check failed\n";
    out.close();
    return 1;
  }
  out.close();
  return 0;
}

// 
// main.cpp ends here
