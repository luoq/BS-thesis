// test_genSubset.cpp --- 
// 
// Filename: test_genSubset.cpp
// Description: test for genSubset
// Author: Luo Qiang
// Created: 03/27/2010 13:33:28
// Version: 
// Last-Updated: 03/27/2010 19:47:21
//           By: Luo Qiang
//     Update #: 11
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "misc.h"
#include <iostream>
using namespace std;

int main()
{
  int			index,addOrRemove,card;
  subsetGenerator	g(6);
  while(!g.end())
    {
      g.next(index,addOrRemove,card);
      if(addOrRemove==1)
	cout<<"Add element "<<index<<endl;
      else
	cout<<"Remove element "<<index<<endl;
    }
  return 0;
}
// 
// test_genSubset.cpp ends here
