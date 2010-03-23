// research.cpp --- 
// 
// Filename: research.cpp
// Description: 
// Author: Luo Qiang
// Created: 03/23/2010 11:08:58
// Version: 
// Last-Updated: 03/23/2010 12:42:09
//           By: Luo Qiang
//     Update #: 54
// Keywords: 

// Commentary: 

// Change log:

// Code:
#include "iSparseMatrix.h"
#include "regular.h"
#include "Timer.h"
int main(int argc,char** argv)
{
  if(argc!=3)
    {
      cerr<<"usage : "<<argv[0]<<" path_to_load path_to_save\n";
      return 1;
    }
  cout<<"loading matrix\n";
  smat<int>	m;
  if(!m.load(argv[1]))
    {
      cerr<<"Cannot open file"<<endl;
      return 2;
    }
  cout<<"done\n\n";
  ofstream	out(argv[2]);
  if(!out)
    {
      cerr<<"Cannot write file\n";
      return 3;
    }

  srand(time(NULL));
  Timer		timer;
  double	t;
  
  int		trytimes;
  cout<<"selecting columns\n";
  vector<int>	selectedCols = aPerfectMatch(m,trytimes);
  if(selectedCols.empty())
    {
      cerr<<"Cannot find a perfect match\n";
      cerr<<"Try again,or check if there is any\n";
      return 4;
    }
  cout<<"done with columns\n";
  cout<<selectedCols;
  cout<<"after "<<trytimes<<" tries\n\n";
  out<<"#research with matrix load from "<<argv[1]<<endl;
  out<<"#selected columns :\n#";
  out<<selectedCols;
  out<<"#after "<<trytimes<<" tries\n";

  int repeat=10;
  out<<"repeat "<<repeat<<" times each\n";
  cout<<"calculating permanent of original matrix "<<repeat<<"times\n";
  timer.tic();
  smat<int> mtemp(m);
  int	P = HPerm(mtemp,0);
  t	  = timer.toc();
  out<<P<<'\t'<<t;
  cout<<"permanent: "<<P<<"\ntime elapsed : "<<t<<"ms ";
  for(int i=0;i<repeat-1;i++)
  {
      mtemp=m;
      timer.tic();
	  HPerm(mtemp,0);
      t	  = timer.toc();
      out<<'\t'<<t;
      cout<<t<<"ms ";
  }
  out<<endl;
  cout<<endl<<endl;

  for(int r=0;r<m.rows();r++)
    {
      m.erase(r,selectedCols[r]);
      cout<<"calculating permanent of matrix with ("<<r<<","<<selectedCols[r]<<") erased\n";
      mtemp=m;
      timer.tic();
      int	P = HPerm(mtemp,0);
      t	  = timer.toc();
      out<<P<<'\t'<<t;
      cout<<"permanent: "<<P<<"\ntime elapsed : "<<t<<"ms ";
	  for(int i=0;i<repeat-1;i++)
	  {
		  mtemp=m;
		  timer.tic();
		  HPerm(mtemp,0);
		  t	  = timer.toc();
		  out<<'\t'<<t;
		  cout<<t<<"ms ";
	  }
	  out<<endl;
	  cout<<endl<<endl;
    }
  return 0;  
}

// 
// research.cpp ends here
