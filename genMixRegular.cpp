#include "Timer.h"
#include "regular.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
bool is_mix_regular(const smat<int>& m,int n,int k,int d)
{
  for(int i=0;i<n-k;i++)
    if(m.row_nnz(i) != d-1||m.col_nnz(i)!=d-1)
      return false;
  for(int i=n-k;i<n;i++)
    if(m.row_nnz(i) != d||m.col_nnz(i)!=d)
      return false;
  return true;
}
int main(int argc,char** argv){
  int	n,k,d,trytimes;
  Timer	timer;
  double time;
  smat<int> matrix;

  if(argc!=5)
    {
      cout<<"Usage: "<<argv[0]<<" n k d pathtosave\n";
      return 1;
    }
  ofstream	out(argv[4]);
  n=atoi(argv[1]);
  k=atoi(argv[2]);
  d=atoi(argv[3]);
  if(!out){
    cout<<"Cannot open file\n";
    return 1;
  }
  
  seed_rand();
  timer.tic();
  matrix = mix_regular(n,k,d,trytimes);
  time   = timer.toc();
  cout<<"tryed "<<trytimes<<" times\n";
  cout<<"Time elapsed: "<<time<<" ms\n";
  out<<matrix;
  out.close();
  //if(!is_mix_regular(matrix,n,k,d))
  //{
  //  cout<<"regular check failed\n";
  //  return 1;
  //}
  return 0;
}
