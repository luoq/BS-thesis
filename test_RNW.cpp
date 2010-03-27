// test_RNW.cpp --- 
// 
// Filename: test_RNW.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/27/2010 15:10:07
// Version: 
// Last-Updated: 03/27/2010 17:31:08
//           By: Luo Qiang
//     Update #: 15
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "iSparseMatrix.h"
#include "iFullMatrix.h"
#include "R-NW.h"
#include "Timer.h"

int main(int argc,char** argv)
{
  smat<int>	sm;
  fmat<int> fm;
  if(!sm.load(argv[1]))
    {
      cout<<"file not found\n";
      return 1;
    }
  fm = sm.full();
#ifdef debug_rnw
  cout<<fm<<endl;
#endif

  Timer		t;
  double	time;
  t.tic();
  int		p = RNW(fm);
  time		  = t.toc();
  cout<<"Time elapsed : "<<time<<" ms\n";
  cout<<"Result : "<<p<<endl;
  return 0;
}
// 
// test_RNW.cpp ends here
