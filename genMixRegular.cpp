#include "Timer.h"
#include "regular.h"
#include <iostream>
#include <fstream>
using	std::cout;
using	std::cin;
using	std::endl;
int main(int argc,char** argv){
  srand(time(NULL));
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
  
  timer.tic();
  matrix = mix_regular(n,k,d,trytimes);
  time   = timer.toc();
  cout<<"tryed "<<trytimes<<" times\n";
  cout<<"Time elapsed: "<<time<<" ms\n";
  out<<matrix;
  //  if(!isregular(matrix,d))
  //  {
  //    cout<<"regular check failed\n";
  //    out.close();
  //    return 1;
  //  }
  out.close();
  return 0;
}
