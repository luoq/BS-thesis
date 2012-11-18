// main.cpp --- 
// 
// Filename: main.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/15/2010 16:38:40
// Version: 
// Last-Updated: 06/14/2010 11:12:10
//           By: Luo Qiang
//     Update #: 73
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
  srand(time(NULL));
  int	n,d,trytimes;
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
  
  timer.tic();
  matrix = regular2(n,d,trytimes);
  time   = timer.toc();
  cout<<"tryed "<<trytimes<<" times\n";
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
