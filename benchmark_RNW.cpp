// benchmark.cpp --- 
// 
// Filename: benchmark.cpp
// Description: benchmark of fucntions
// Author: Luo Qiang
// Created: 03/19/2010 12:55:51
// Version: 
// Last-Updated: 03/27/2010 19:44:57
//           By: Luo Qiang
//     Update #: 54
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "Timer.h"
#include "regular.h"
#include "iSparseMatrix.h"
#include "iFullMatrix.h"
#include "R-NW.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main()
{
  Timer		timer;
  double	t=0;
  ofstream	out("../data/benchmark-RNW.data",ofstream::app);
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }

  srand(time(NULL));
  int		n,d,trytimes,repeat;
  d	 = 3;
  repeat = 5;
  smat<int>	matrix;
  fmat<int>	fm;
  int P;
  //for odd n,no possible regular matrix
  out<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out<<"# size\ttime\tPermanent\n";
  for(int n=26;n<=30;n+=2)
    {
      out<<n<<'\t';
    for(int i=0;i<repeat;++i)
      {
	matrix = regular(n,d,trytimes);
	fm=matrix.full();
	timer.tic();
	P=RNW(fm);
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
