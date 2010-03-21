// test_HPerm.cpp --- 
// 
// Filename: test_HPerm.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version: 
// Last-Updated: 03/21/2010 19:32:10
//           By: Luo Qiang
//     Update #: 45
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"
int main()
{
  //smat<int>	m(3,3);
  //m.set(0,0,5);
  //m.set(0,1,2);
  //m.set(1,1,3);
  //m.set(1,0,7);
  //m.set(2,2,2);
  smat<int>	m;
  //m.load_octave("../data/F30.data");
  m.load_octave("test.data");

  smat<int>	mcopy(m);
#ifdef plot
  cout<<"digraph \"call-tree\" {\nnode [shape=box,style=filled,fillcolor=yellow]; \nrankdir=BT;ranksep=0.2;\n";
#endif
  int p=HPerm(mcopy);
#ifdef plot
  cout<<"}\n";
#endif
  cout<<p<<endl;
  return 0;
}
// 
// test_HPerm.cpp ends here
