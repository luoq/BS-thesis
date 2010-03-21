// test_load.cpp --- 
// 
// Filename: test_load.cpp
// Description: test the load_octave function
// Author: Luo Qiang
// Created: 03/21/2010 13:14:33
// Version: 
// Last-Updated: 03/21/2010 13:17:11
//           By: Luo Qiang
//     Update #: 3
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"

int main()
{
  smat<int>	m;
  m.load_octave("../data/F30.data");
  cout<<m;
  return 0;
}
// 
// test_load.cpp ends here
