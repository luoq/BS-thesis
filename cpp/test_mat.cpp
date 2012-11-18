// test.cpp ---
//
// Filename: test.cpp
// Description: test for iSparseMatrix
// Author: Luo Qiang
// Created: 03/17/2010 16:42:44
// Version:
// Last-Updated: 03/22/2010 15:38:23
//           By: Luo Qiang
//     Update #: 75
// Keywords:

// Commentary:

// Change log:

// Code:
#include "permanent.h"
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
	//cout<<m<<endl;
	m.print();
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
	//cout<<m<<endl;
	m.print();
}
void test_erase_row(smat<int> &m)
{
	cout<<"erase row\n";
	cout<<"Input row :\n";
	int r;
	cin>>r;
	m.erase_row(r);
	//cout<<m<<endl;
	m.print();
}
void test_erase_col(smat<int> &m)
{
	cout<<"erase col\n";
	cout<<"Input column :\n";
	int c;
	cin>>c;
	m.erase_col(c);
	//cout<<m<<endl;
	m.print();
}
void test_col_nnzs(smat<int> &m)
{
	vector<int>	col_nnzs = m.col_nnzs();
	for(int c=0;c<col_nnzs.size();c++)
		cout<<"column "<<c<<" has "<<col_nnzs[c]<<" elements.\n";
}
void test_eliminate2(smat<int> &m)
{
	cout<<"eliminate 2 cols\n"
		<<"Input row,c1,c2,v1,v2\n";
	int row,c1,c2;
	int v1,v2;
	cin>>row>>c1>>c2>>v1>>v2;
	eliminate2(m,row,c1,c2,v1,v2);
	m.print();
	test_col_nnzs(m);
}
void test_eliminate1(smat<int> &m)
{
	cout<<"eliminate 1\n"
		<<"Input row,col\n";
	int row,col;
	cin>>row>>col;
	eliminate1(m,row,col);
	m.print();
	test_col_nnzs(m);
}
void test_eliminate2T(smat<int> &m)
{
	cout<<"eliminate 2 rows\n"
		<<"Input col,r1,r2,v1,v2\n";
	int col,r1,r2;
	int v1,v2;
	cin>>col>>r1>>r2>>v1>>v2;
	eliminate2T(m,col,r1,r2,v1,v2);
	m.print();
	test_col_nnzs(m);
}
int main()
{
	smat<int>	m;
	if(!m.load("../data/test.data"))
	{
		cout<<"file not found.\n";
		return 1;
	}
	//cout<<m;
	m.print();

	while(1)
	{
		cout<<"set(s),get(g),erase(e),erase_row(r),erase_col(c),print(p)\n"
			<<"col_nnzs(x),eliminate2(2),eliminate1(1),eliminatet(t)?\n";
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
			case 'r':
				test_erase_row(m);
				break;
			case 'c':
				test_erase_col(m);
				break;
			case 'x':
				test_col_nnzs(m);
				break;
			case '2':
				test_eliminate2(m);
				break;
			case '1':
				test_eliminate1(m);
				break;
			case 't':
				test_eliminate2T(m);
				break;
			case 'p':
				//cout<<m;
				m.print();
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
