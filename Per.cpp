// test_H.cpp ---
//
// Filename: test_H.cpp
// Description:
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version:
// Last-Updated: 12/06/2010 14:27:01
//           By: Luo Qiang
//     Update #: 106
// Keywords:

// Commentary:

// Change log:

// Code:

#include "permanent.h"
#include "iSparseMatrix.h"
#include "iFullMatrix.h"
#include "Timer.h"
template<typename T>
void test(T (*f)(smat<T>&,int),smat<T> m,string name)
{
  Timer t;
  double time;
  T p;
  int size=m.rows();

  t.tic();
  p = f(m,1);
  time = t.toc();
#ifdef plot
  cout<<"}\n";
  cerr<<"Time elapsed : "<<time<<" s\n";
  cerr<<"Result : "<<p<<endl;
#else
  //cout<<name<<"\n";
  //cout<<"Time elapsed : "<<time<<" s\n";
  //cout<<"Result : "<<p<<endl<<endl;
  cout<<size<<'\t'<<p<<'\t'<<time<<endl;
#endif
}
template<typename T>
void test_RNW(smat<T> m,string name)
{
  Timer t;
  double time;
  RNW_type p;
  int size=m.rows();

  t.tic();
  p = RNW(m.full());
  time = t.toc();
  cout<<size<<'\t'<<p<<'\t'<<time<<endl;
}
int main(int argc,char** argv)
{
  typedef long long Num;
  smat<Num>	m;
  if(!m.load(argv[1]))
    {
      cout<<"file not found\n";
      return 1;
    }
#ifdef plot
  cout<<"digraph \"call-tree\" {\nnode [shape=box,style=filled,fillcolor=yellow]; \nrankdir=BT;\n";
#endif
  //test(&IDEM0<Num>,m,"IDEM0");
  //test(&IDEM<Num>,m,"IDEM");
  generate_travel_order();
  test(&H<Num>,m,"H");
  //test(&IDEM3<Num>,m,"IDEM3");
  //test(&H3<Num>,m,"H3");
  //test(&DEM<Num>,m,"DEM");
  //test(&DEM2<Num>,m,"DEM2");
  //test(&DEMiter<Num>,m,"DEMiter");
  //test_RNW(m,"RNW");
  return 0;
}
//
// test_H.cpp ends here
