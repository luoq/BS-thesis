// research.cpp ---
//
// Filename: research.cpp
// Description:
// Author: Luo Qiang
// Created: 03/23/2010 11:08:58
// Version:
// Last-Updated: 05/22/2010 10:51:43
//           By: Luo Qiang
//     Update #: 83
// Keywords:

// Commentary:

// Change log:

// Code:
#include "permanent.h"
#include <cstring>
int main(int argc,char** argv)
{
  if(argc!=4)
    {
      cerr<<"usage : "<<argv[0]<<" path/to/matrix #elements #repeat-time\n";
      return 1;
    }
  smat<int>	m;
  if(!m.load(argv[1]))
    {
      cerr<<"Cannot open file"<<endl;
      return 2;
    }

  int k=atoi(argv[2]),repeat=atoi(argv[3]);
  int trytimes;
  vector<int> rows,cols;
  rows.reserve(k);
  cols.reserve(k);

  srand(time(NULL));

  for(int i=0;i<repeat;i++)
  {
    selectElements(m,k,rows,cols,trytimes);
    cout<<rows<<endl;
    cout<<cols<<endl;
  }
  return 0;
}

//
// research.cpp ends here
