// test_H.cpp --- 
// 
// Filename: test_H.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/21/2010 15:27:10
// Version: 
// Last-Updated: 04/21/2010 09:29:08
//           By: Luo Qiang
//     Update #: 89
// Keywords: 

// Commentary: 

// Change log:

// Code:

#include "permanent.h"
#include "iSparseMatrix.h"
#include "iFullMatrix.h"
#include "Timer.h"
	template<typename T>
void test(T (*f)(smat<T>&,int),smat<T> m,string name)
{
	Timer t;
	double time;
	T p;
	int size=m.rows();

	t.tic();
	p = f(m,1);
	time = t.toc();
#ifdef plot
	cout<<"}\n";
	cerr<<"Time elapsed : "<<time<<" ms\n";
	cerr<<"Result : "<<p<<endl;
#else
	//cout<<name<<"\n";
	//cout<<"Time elapsed : "<<time<<" ms\n";
	//cout<<"Result : "<<p<<endl<<endl;
	cout<<size<<'\t'<<p<<'\t'<<time<<endl;
#endif
}
template<typename T>
void test_RNW(smat<T> m,string name)
{
	Timer t;
	double time;
	T p;

	t.tic();
	p = RNW(m.full());
	time = t.toc();
	cout<<name<<"\n";
	cout<<"Time elapsed : "<<time<<" ms\n";
	cout<<"Result : "<<p<<endl<<endl;
}
int main(int argc,char** argv)
{
	typedef long long ll;
	smat<int>	m;
	if(!m.load(argv[1]))
	{
		cout<<"file not found\n";
		return 1;
	}
#ifdef plot
	cout<<"digraph \"call-tree\" {\nnode [shape=box,style=filled,fillcolor=yellow]; \nrankdir=BT;\n";
#endif
	//test(&IDEM0<int>,m,"IDEM0");
	test(&IDEM<int>,m,"IDEM");
	//generate_travel_order();
	//test(&H<int>,m,"H");
	//test(&IDEM3<int>,m,"IDEM3");
	//test(&H3<int>,m,"H3");
	//test(&DEM<int>,m,"DEM");
	//test(&DEM2<int>,m,"DEM2");
	//test(&DEMiter<int>,m,"DEMiter");
	//test_RNW(m,"RNW");
	return 0;
}
// 
// test_H.cpp ends here
