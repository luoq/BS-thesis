// misc.cpp --- 
// 
// Filename: misc.cpp
// Description: miscellaneous functions
// Author: Luo Qiang
// Created: 03/23/2010 10:33:04
// Version: 
// Last-Updated: 03/27/2010 17:20:14
//           By: Luo Qiang
//     Update #: 33
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "misc.h"
int randint(int n){
  return rand()%n;
}

subsetGenerator::subsetGenerator(int n)
  :n(n),_k(0),_end(0)
{
  set=new bool[n];
}
subsetGenerator::~subsetGenerator()
{
  delete[] set;
}
void subsetGenerator::next(int &nextIndex,int &addOrRemove,int& k)
{
  if(_k%2==0)
    nextIndex	 = 0;
  else
    {
      for(nextIndex=0;set[nextIndex]!=true;nextIndex++);
      nextIndex++;
    }
  if(set[nextIndex]==0)
    addOrRemove	 = 1;
  else
    addOrRemove	 = -1;
  set[nextIndex] = 1-set[nextIndex];
  _k		 = _k+2*set[nextIndex]-1;
  k=_k;
  if(_k==set[n-1])
    _end=1;
}
// 
// rand.cpp ends here
