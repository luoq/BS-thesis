// misc.cpp ---
//
// Filename: misc.cpp
// Description: miscellaneous functions
// Author: Luo Qiang
// Created: 03/23/2010 10:33:04
// Version:
// Last-Updated: 05/21/2010 09:06:49
//           By: Luo Qiang
//     Update #: 51
// Keywords:

// Commentary:

// Change log:

// Code:
#include "misc.h"

void seed_rand()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   srand(tv.tv_sec * tv.tv_usec);
}
int randint(int n){
  return rand()%n;
}
vector<int> chooseKfromN(int n,int k)
{

  vector<int> ret;
  ret.reserve(k);
  if(k==0)
    return ret;
  for(int i=0;ret.size()<k;i++)
    {
      if(rand()%(n-i)<(k-ret.size()))
	ret.push_back(i);
    }
  return ret;
}

subsetGenerator::subsetGenerator(int n)
  :n(n),_k(0),_end(0)
{
  set =	new bool[n];
  for(int i=0;i<n;i++)
    set[i]=0;
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
  k		 = _k;
  if(_k==set[n-1])
    _end	 = 1;
#ifdef debug_rnw
  if(addOrRemove==1)
    cout<<"Add element "<<nextIndex<<endl;
  else
    cout<<"Remove element "<<nextIndex<<endl;
  if(_end==1)
    cout<<"END\n";
#endif
}
//
// rand.cpp ends here
