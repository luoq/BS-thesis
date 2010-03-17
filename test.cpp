// test.cpp ---
//
// Filename: test.cpp
// Description: test for iSparseMatrix
// Author: Luo Qiang
// Created: 03/17/2010 16:42:44
// Version:
// Last-Updated: 03/17/2010 22:34:55
//           By: Luo Qiang
//     Update #: 23
// Keywords:

// Commentary:

// Change log:

// Code:
#include "iSparseMatrix.h"

char getch() {
  char a;
  cin >> a;
  cin.ignore(1024, '\n');
  return a;
}
void test_set(smat<int> &m)
{
  int r,c,e;
  cout<<"set element\n";
  cout<<"Input row,column,element"<<endl;
  cin>>r>>c>>e;
  m.set(r,c,e);
  cout<<m<<endl;
}
void test_get(const smat<int> &m)
{
  cout<<"get element\n";
  cout<<"Input row,column :\n";
  int r,c;
  cin>>r>>c;
  cout<<m(r,c)<<endl;
}
int main()
{
  smat<int>	m(100,100,3);
  while(1)
    {
      cout<<"set(s),get(g),print(p)?\n";
      char	choice = getch();
      switch(choice)
	{
	case 's':
	  test_set(m);
	  break;
	case 'g':
	  test_get(m);
	  break;
	case 'p':
	  cout<<m;
	  break;
	case 'q':
	  return 0;
	default:
	  break;
	}
    }
}



//
// test.cpp ends here
