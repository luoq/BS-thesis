// test.cpp ---
//
// Filename: test.cpp
// Description: test for iSparseMatrix
// Author: Luo Qiang
// Created: 03/17/2010 16:42:44
// Version:
// Last-Updated: 03/19/2010 20:56:19
//           By: Luo Qiang
//     Update #: 60
// Keywords:

// Commentary:

// Change log:

// Code:
#include "iSparseMatrix.h"
#include "regular.h"

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
void test_erase(smat<int> &m)
{
  cout<<"erase element\n";
  cout<<"Input row,column :\n";
  int r,c;
  cin>>r>>c;
  m.erase(r,c);
  cout<<m<<endl;
}
int main()
{
  smat<int>	m(100,100,10);
  //m.set(1,1,1);
  //m.set(1,1,1);
  //m.set(2,0,1);
  //m.set(0,2,1);
  //m.set(1,2,1);
  //m.set(2,1,1);
  //cout<<m(2,1)<<endl;
  //cout<<m;
  //return 0;

  while(1)
    {
      cout<<"set(s),get(g),erase(e),print(p)?\n";
      char	choice = getch();
      switch(choice)
	{
	case 's':
	  test_set(m);
	  break;
	case 'g':
	  test_get(m);
	  break;
	case 'e':
	  test_erase(m);
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
