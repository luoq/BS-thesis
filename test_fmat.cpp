// test_fmat.cpp --- 
// 
// Filename: test_fmat.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/27/2010 15:31:42
// Version: 
// Last-Updated: 03/27/2010 15:51:59
//           By: Luo Qiang
//     Update #: 8
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "iFullMatrix.h"
#include <iostream>
using namespace std;
int main()
{
  fmat<int>	m(4,5);
  cout<<m<<endl;
  m(2,2) += 10;
  m(2,2) += 10;
  cout<<m<<endl;
  return 0;
}
// 
// test_fmat.cpp ends here
