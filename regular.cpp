// regular.cpp ---
//
// Filename: regular.cpp
// Description:
// Author: Luo Qiang
// Created: 03/15/2010 10:04:55
// Version:
// Last-Updated: 04/20/2010 15:01:56
//           By: Luo Qiang
//     Update #: 176
// Keywords:

// Commentary:

// Change log:

// Code:
#include "regular.h"

//get the group of a point
int	group(int point,int d);
void	printMatrix(const smat<int> &matrix);

smat<int> regular(int n,int d,int &trytimes){
  smat<int> matrix(n,n,d);
  trytimes=1;
  if(n*d%2!=0){
    return matrix;
  }
#ifdef debug
  cout<<matrix<<endl;
#endif
  vector<int> points(n*d);
  vector<int> suitablePoints,suitablePointsIndex;
 tryagain:
  for(unsigned i=0;i!=points.size();++i){
    points[i]=i;
  }
  while(!points.empty()){
    int		index1				  = randint(points.size());
    int		group1				  = group(points[index1],d);
#ifdef debug
    //cout<<"matrix: \n";
    //printMatrix(matrix);
    cout<<"points :\n";
    printVector(points);
    //cout<<"1st point: "<<points[index1]<<endl;
#endif
    suitablePoints.clear();
    suitablePointsIndex.clear();
    for(unsigned i=0;i!=points.size();++i){
      int group2 = group(points[i],d);
      if(group2!=group1 && matrix(group1,group2) == 0){
	suitablePoints.push_back(points[i]);
	suitablePointsIndex.push_back(i);
      }
    }
    if(suitablePoints.empty()){
#ifdef debug
      cout<<"try again"<<endl;
#endif
      matrix.clear();
      points.resize(n*d);
      trytimes++;
      goto tryagain;
    }
    int index2		      = randint(suitablePoints.size());
    int group2		      = group(suitablePoints[index2],d);
    matrix.set(group1,group2,1);
    matrix.set(group2,group1,1);
#ifdef debug
    cout<<"points to be deleted:\n";
    cout<<points[index1]<<' '<<suitablePoints[index2]<<endl;
    cout<<"add: ("<<group1<<','<<group2<<")\n";
    if(matrix(group1,group2) !=	1)
      {
	cout<<matrix;
	exit(1);
      }
    cout<<"add: ("<<group2<<','<<group1<<")\n";
    if(matrix(group2,group1) !=	1)
      {
	cout<<matrix;
	exit(1);
      }
#endif
    points.erase(points.begin()+index1);
    //NOTE,when index1<suitablePointsIndex[index2],the index of second point
    //changed as the erase of first point
    if(index1<suitablePointsIndex[index2])
      points.erase(points.begin()+suitablePointsIndex[index2]-1);
    else
      points.erase(points.begin()+suitablePointsIndex[index2]);
  }
  return matrix;
}
int group(int point,int d){
  return int(point/d);
}
// regular.cpp ends here
