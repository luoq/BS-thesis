// test_HPerm.cpp --- 
// 
// Filename: test_HPerm.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version: 
// Last-Updated: 03/22/2010 14:40:57
//           By: Luo Qiang
//     Update #: 73
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"
#include "Timer.h"
int main(int argc,char** argv)
{
  //smat<int> m(4,4,4);
  //m.set(0,1,1);
  //m.set(0,2,2);
  //m.set(0,3,3);
  //m.set(1,1,1);
  //m.set(1,2,2);
  //m.set(1,3,3);
  //m.set(2,0,2);
  //m.set(2,2,1);
  //m.set(3,0,1);
  //m.set(3,3,1);

  
  smat<long long>	m;
  if(!m.load(argv[1]))
    {
      cout<<"file not found\n";
      return 1;
    }
#ifdef plot
  cout<<"digraph \"call-tree\" {\nnode [shape=box,style=filled,fillcolor=yellow]; \nrankdir=BT;ranksep=0.2;\n";
#endif

  Timer t;
  double time;
  t.tic();
  long long	p = HPerm(m);
  time	  = t.toc();
#ifdef plot
  cout<<"}\n";
  cerr<<"Time elapsed : "<<time<<" ms\n";
  cerr<<"Result : "<<p<<endl;
#else
  cout<<"Time elapsed : "<<time<<" ms\n";
  cout<<"Result : "<<p<<endl;
#endif
  return 0;
}
// 
// test_HPerm.cpp ends here
