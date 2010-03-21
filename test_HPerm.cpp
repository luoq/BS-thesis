// test_HPerm.cpp --- 
// 
// Filename: test_HPerm.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version: 
// Last-Updated: 03/21/2010 16:06:17
//           By: Luo Qiang
//     Update #: 12
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"
int main()
{
  smat<int>	m(1,1);
  m.set(1,1,5);
  cout<<m;
  //m.load_octave("../data/F30.data");
  smat<int> mcopy(m);
  cout<<HPerm(mcopy)<<endl;
  return 0;
}
// 
// test_HPerm.cpp ends here
