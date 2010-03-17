// test.cpp --- 
// 
// Filename: test.cpp
// Description: test for iSparseMatrix 
// Author: Luo Qiang
// Created: 03/17/2010 16:42:44
// Version: 
// Last-Updated: 03/17/2010 20:34:01
//           By: Luo Qiang
//     Update #: 12
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "iSparseMatrix.h"
void test_set(smat<int> &m)
{
  int r,c,e;
  while(1)
    {
      cout<<"Input row,column,element"<<endl;
      cin>>r>>c>>e;
      m.set(r,c,e);
      cout<<m<<endl;
    }
}
int main()
{
  smat<int>	m(10,10,3);
  cout<<m<<endl;
  test_set(m);
  return 0;
}



// 
// test.cpp ends here
