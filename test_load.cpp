// test_load.cpp --- 
// 
// Filename: test_load.cpp
// Description: test the load_octave function
// Author: Luo Qiang
// Created: 03/21/2010 13:14:33
// Version: 
// Last-Updated: 03/22/2010 09:27:16
//           By: Luo Qiang
//     Update #: 7
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iSparseMatrix.h"

int main()
{
  smat<int>	m;
  if(!m.load("../data/matrix.data"))
    return 1;
  cout<<m;
  return 0;
}
// 
// test_load.cpp ends here
