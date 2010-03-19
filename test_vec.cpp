// test.cpp ---
//
// Filename: test.cpp
// Description: test for iSparseMatrix
// Author: Luo Qiang
// Created: 03/17/2010 16:42:44
// Version:
// Last-Updated: 03/19/2010 16:16:07
//           By: Luo Qiang
//     Update #: 62
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
void test_set(svec<int> &v)
{
  int	i,e;
  cout<<"set element\n"
      <<"Input index,element"<<endl;
  cin>>i>>e;
  v.set(i,e);
  cout<<v<<endl;
}
void test_get(const svec<int> &v)
{
  cout<<"get element\n"
      <<"Input index :\n";
  int i;
  cin>>i;
  cout<<v(i)<<endl;
}
void test_erase(svec<int> &v)
{
  cout<<"erase element\n"
      <<"Input index:\n";
  int i;
  cin>>i;
  v.erase(i);
  cout<<v<<endl;
}
int main()
{
  svec<int>	v(100,10);

  while(1)
    {
      cout<<"set(s),get(g),erase(e),print(p)?\n";
      char	choice = getch();
      switch(choice)
	{
	case 's':
	  test_set(v);
	  break;
	case 'g':
	  test_get(v);
	  break;
	case 'e':
	  test_erase(v);
	  break;
	case 'p':
	  cout<<v;
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
