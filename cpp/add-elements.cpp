// add-elements.cpp --- 
// 
// Filename: add-calcu.cpp
// Description: 
// Author: Luo Qiang
// Created: 05/22/2010 10:22:23
// Version: 
// Last-Updated: 05/22/2010 10:58:47
//           By: Luo Qiang
//     Update #: 11
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "permanent.h"
#include <cstring>
int main(int argc, char *argv[])
{
  if(argc!=3)
    {
      cerr<<"usage : "<<argv[0]<<"matrix_path repeat_time\n";
      return 1;
    }
  smat<int>	m;
  if(!m.load(argv[1]))
    {
      cerr<<"Cannot open file"<<endl;
      return 2;
    }
  int		repeat = atoi(argv[2]);
  int		trytimes;
  vector<int>	cols;
  cols.reserve(m.cols());
  //seed_rand();
  
  for(int i=0;i<repeat;i++)
  {
    addElements(m,cols,trytimes);
    cout<<cols<<endl;
  }
  return 0;
}
// 
// add-elements.cpp ends here
