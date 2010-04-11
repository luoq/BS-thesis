// test_H.cpp --- 
// 
// Filename: test_H.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version: 
// Last-Updated: 04/05/2010 10:41:10
//           By: Luo Qiang
//     Update #: 86
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"
#include "Timer.h"
int main(int argc,char** argv)
{
  smat<long long>	m,mtemp;
  long long p;
  if(!m.load(argv[1]))
    {
      cout<<"file not found\n";
      return 1;
    }
#ifdef plot
  cout<<"digraph \"call-tree\" {\nnode [shape=box,style=filled,fillcolor=yellow]; \nrankdir=BT;\n";
#endif

  Timer t;
  double time;

//  mtemp=m;
//  t.tic();
//  p = IDEM0(mtemp);
//  time		  = t.toc();
//#ifdef plot
//  cout<<"}\n";
//  cerr<<"Time elapsed : "<<time<<" ms\n";
//  cerr<<"Result : "<<p<<endl;
//#else
//  cout<<"using IDEM0\n";
//  cout<<"Time elapsed : "<<time<<" ms\n";
//  cout<<"Result : "<<p<<endl;
//#endif


  p=0;
  mtemp=m;
  t.tic();
  p = IDEM(mtemp);
  time		  = t.toc();
#ifdef plot
  cout<<"}\n";
  cerr<<"Time elapsed : "<<time<<" ms\n";
  cerr<<"Result : "<<p<<endl;
#else
  cout<<"using IDEM\n";
  cout<<"Time elapsed : "<<time<<" ms\n";
  cout<<"Result : "<<p<<endl;
#endif

  p=0;
  mtemp=m;
  t.tic();
  p = H(mtemp);
  time		  = t.toc();
#ifdef plot
  cout<<"}\n";
  cerr<<"Time elapsed : "<<time<<" ms\n";
  cerr<<"Result : "<<p<<endl;
#else
  cout<<"using hybrid\n";
  cout<<"Time elapsed : "<<time<<" ms\n";
  cout<<"Result : "<<p<<endl;
#endif

  p=0;
  mtemp=m;
  t.tic();
  p = DEM(mtemp);
  time		  = t.toc();
#ifdef plot
  cout<<"}\n";
  cerr<<"Time elapsed : "<<time<<" ms\n";
  cerr<<"Result : "<<p<<endl;
#else
  cout<<"using DEM\n";
  cout<<"Time elapsed : "<<time<<" ms\n";
  cout<<"Result : "<<p<<endl;
#endif
  return 0;
}
// 
// test_H.cpp ends here
