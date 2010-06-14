// misc.cpp ---
//
// Filename: misc.cpp
// Description: miscellaneous functions
// Author: Luo Qiang
// Created: 03/23/2010 10:33:04
// Version:
// Last-Updated: 06/14/2010 09:50:01
//           By: Luo Qiang
//     Update #: 73
// Keywords:

// Commentary:

// Change log:

// Code:
#include "misc.h"

MTRand mtrand;
//void seed_rand()
//{
//   struct timeval tv;
//   gettimeofday(&tv, NULL);
//   srand(tv.tv_sec * tv.tv_usec);
//}
int randint(int n){
  //return rand()%n;
  return mtrand.randInt(n);
}
double rand1()
{
  //return rand()/double(RAND_MAX);
  return mtrand.randExc(1.0);
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
int choose1_with_weight(vector<int> w)
{
  double	S  = 0;
  for(int i=0;i<w.size();i++)
    S		  += w[i];
  if(S<=0)
    return -1;			//No selection
  double	p  = rand1();
  double A=0.0;
  for(int i=0;i<w.size();i++)
    {
      A	+= w[i];
      if(p<A/S)
	return i;
    }
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
