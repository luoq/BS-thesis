// benchmark.cpp --- 
// 
// Filename: benchmark.cpp
// Description: benchmark of fucntions
// Author: Luo Qiang
// Created: 03/19/2010 12:55:51
// Version: 
// Last-Updated: 04/20/2010 14:08:56
//           By: Luo Qiang
//     Update #: 52
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "Timer.h"
#include "regular.h"
#include "permanent.h"
#include <iostream>
#include <fstream>
#include <cstring>
using	std::cout;
using	std::cin;
using	std::endl;
	template<typename T>
void benchmark(T (*f)(smat<T>&,int),ofstream& out)
{
	Timer		timer;
	double	t=0;
	int		n,d,trytimes,repeat;
	d	 = 3;
	repeat = 50;
	smat<int>	matrix;
	int P;
	//for odd n,no possible regular matrix
	out<<"# bechmark for regular matrix generation with d = "<<d<<endl;
	out<<"# size\ttime\tPermanent\n";
	for(int n=4;n<=50;n+=2)
	{
		out<<n<<'\t';
		for(int i=0;i<repeat;++i)
		{
			matrix = regular(n,d,trytimes);
			timer.tic();
			P=f(matrix,1);
			t = timer.toc();
			out<<t<<'\t'<<P<<'\t';
		}
		out<<endl;
	}
	out.close();
}
void benchmark_RNW(ofstream& out)
{
	Timer		timer;
	double	t=0;
	int		n,d,trytimes,repeat;
	d	 = 3;
	repeat = 50;
	smat<int>	matrix;
	int P;
	//for odd n,no possible regular matrix
	out<<"# bechmark for regular matrix generation with d = "<<d<<endl;
	out<<"# size\ttime\tPermanent\n";
	for(int n=4;n<=20;n+=2)
	{
		out<<n<<'\t';
		for(int i=0;i<repeat;++i)
		{
			matrix = regular(n,d,trytimes);
			timer.tic();
			P=RNW(matrix.full());
			t = timer.toc();
			out<<t<<'\t'<<P<<'\t';
		}
		out<<endl;
	}
	out.close();
}
int main(int argc,char** argv)
{
	if(argc!=3)
	{
		cout<<"usage: "<<argv[0]<<" method save-path\n";
		return 1;
	}
	ofstream	out(argv[2]);
	if(!out){
		cout<<"Cannot open file\n";
		return 1;
	}
	srand(time(NULL));
	if(strcmp(argv[1],"IDEM")==0)
		benchmark(&IDEM<int>,out);
	else if(strcmp(argv[1],"IDEM3")==0)
		benchmark(&IDEM3<int>,out);
	else if(strcmp(argv[1],"H")==0)
		benchmark(&H<int>,out);
	else if(strcmp(argv[1],"H3")==0)
		benchmark(&H3<int>,out);
	else if(strcmp(argv[1],"DEM")==0)
		benchmark(&DEM<int>,out);
	else if(strcmp(argv[1],"DEMiter")==0)
		benchmark(&DEMiter<int>,out);
	else if(strcmp(argv[1],"RNW")==0)
		benchmark_RNW(out);
	else
	{
		cout<<"No such method\n";
		return 2;
	}
	return 0;
}
// 
// benchmark.cpp ends here
