// benchmark.cpp --- 
// 
// Filename: benchmark.cpp
// Description: benchmark of fucntions
// Author: Luo Qiang
// Created: 03/19/2010 12:55:51
// Version: 
// Last-Updated: 04/10/2010 11:27:04
//           By: Luo Qiang
//     Update #: 51
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "Timer.h"
#include "regular.h"
#include "iSparseMatrix.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main()
{
  Timer		timer;
  double	t=0;
  ofstream	out("../data/benchmark-DEM-inline.data",ofstream::app);
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }

  srand(time(NULL));
  int		n,d,trytimes,repeat;
  d	 = 3;
  repeat = 5;
  smat<int>	matrix;
  int P;
  //for odd n,no possible regular matrix
  out<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out<<"# size\ttime\tPermanent\n";
  for(int n=4;n<=50;n+=2)
    {
      out<<n<<'\t';
    for(int i=0;i<repeat;++i)
      {
	matrix = regular(n,d,trytimes);
	timer.tic();
	P=DEM(matrix);
	t = timer.toc();
	out<<t<<'\t'<<P<<'\t';
      }
    out<<endl;
    }
  out.close();
  return 0;
}
// 
// benchmark.cpp ends here
