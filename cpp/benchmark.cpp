// benchmark.cpp --- 
// 
// Filename: benchmark.cpp
// Description: benchmark of fucntions
// Author: Luo Qiang
// Created: 03/19/2010 12:55:51
// Version: 
// Last-Updated: 04/20/2010 16:25:22
//           By: Luo Qiang
//     Update #: 76
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "Timer.h"
#include "regular.h"
#include "permanent.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main()
{
  Timer		timer;
  double	t=0;
  ofstream	out("../data/benchmark-H-8.data",ofstream::app);
  ofstream	out1("../data/benchmark-IDEM.data",ofstream::app);
  ofstream	out2("../data/benchmark-IDEM0.data",ofstream::app);
  ofstream	out3("../data/benchmark-RNW.data",ofstream::app);
  if(!out||!out1||!out2){
    cout<<"Cannot open file\n";
    return 1;
  }

  srand(time(NULL));
  int		n,d,trytimes,repeat;
  int max1=50;//for H IDEM IDEM0
  int max2=30;//for R-NW
  d	 = 3;
  repeat = 10;
  smat<int>	m,m1,m2;
  fmat<int> fm;
  int P;
  //for odd n,no possible regular matrix
  out<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out<<"# size\ttime\tPermanent\n";
  out1<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out1<<"# size\ttime\tPermanent\n";
  out2<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out2<<"# size\ttime\tPermanent\n";
  out3<<"# bechmark for regular matrix generation with d = "<<d<<endl;
  out3<<"# size\ttime\tPermanent\n";
  for(int n=4;n<=max1;n+=2)
    {
      out<<n<<'\t';
      out1<<n<<'\t';
      out2<<n<<'\t';
      if(n<=max2)
	out3<<n<<'\t';
    for(int i=0;i<repeat;++i)
      {
	m = regular(n,d,trytimes);
	m1=m;m2=m;
	if(n<=max2)
	  {
	    fm=m.full();
	    timer.tic();
	    P=RNW(fm);
	    t = timer.toc();
	    out3<<t<<'\t'<<P<<'\t';
	  }

	timer.tic();
	P = H(m);
	t = timer.toc();
	out<<t<<'\t'<<P<<'\t';

	timer.tic();
	P = IDEM(m1);
	t = timer.toc();
	out1<<t<<'\t'<<P<<'\t';

	timer.tic();
	P = IDEM0(m2);
	t = timer.toc();
	out2<<t<<'\t'<<P<<'\t';	
      }
    out<<endl;
    out1<<endl;
    out2<<endl;
    if(n<=max2)
      out3<<endl;
    }
  out.close();
  out1.close();
  out2.close();
  out3.close();
  return 0;
}
// 
// benchmark.cpp ends here
