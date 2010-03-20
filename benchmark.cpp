// benchmark.cpp --- 
// 
// Filename: benchmark.cpp
// Description: benchmark of fucntions
// Author: Luo Qiang
// Created: 03/19/2010 12:55:51
// Version: 
// Last-Updated: 03/19/2010 13:30:34
//           By: Luo Qiang
//     Update #: 17
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "Timer.h"
#include "regular.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main()
{
  srand(time(NULL));
  int		n,d,repeat;
  Timer		timer;
  double	time;
  ofstream	out("benchmark.data");
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  d	 = 3;
  repeat = 10;
  smat<int>	matrix;
  //for odd n,no possible regular matrix
  out<<"# bechmark for regular matrix generation with d = "<<d<<
    "\n # from n=4 to 100,repeated "<<repeat<<" times each \n";
  for(int n=4;n<=1000;n+=2)
    {
      out<<n<<'\t';
    for(int i=0;i<repeat;++i)
      {
	timer.tic();
	matrix = regular(n,d);
	time   = timer.toc();
	out<<time<<'\t';
      }
    out<<endl;
    }
  out.close();
  return 0;
}



// 
// benchmark.cpp ends here
