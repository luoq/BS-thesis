// regular.cpp ---
//
// Filename: regular.cpp
// Description:
// Author: Luo Qiang
// Created: 03/15/2010 10:04:55
// Version:
// Last-Updated: 03/18/2010 15:35:09
//           By: Luo Qiang
//     Update #: 137
// Keywords:

// Commentary:

// Change log:

// Code:
#include "regular.h"
//#define debug

//generate a random integer from 0 to n
inline int			randint(int n);
//get the group of a point
inline int			group(int point,int d);
template<typename T> void	printVector(const vector<T> &vect);
void				printMatrix(const smat<int> &matrix);

smat<int> regular(int n,int d){
  //n*d elements
  smat<int> matrix(n,n,n*d);
  if(n*d%2!=0){
    return matrix;
  }
#ifdef debug
  cout<<matrix<<endl;
#endif
  vector<int> points(n*d);
  vector<int> suitablePoints,suitablePointsIndex;
 tryagain:
  for(int i=0;i!=points.size();++i){
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
    for(int i=0;i!=points.size();++i){
      int	group2				  = group(points[i],d);
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
      goto tryagain;
    }
    int index2 = randint(suitablePoints.size());
    int group2 = group(suitablePoints[index2],d);
    matrix.set(group1,group2,1);
    matrix.set(group2,group1,1);
#ifdef debug
    cout<<"points to be deleted:\n";
    cout<<points[index1]<<' '<<suitablePoints[index2]<<endl;
    cout<<"add: ("<<group1<<','<<group2<<")\n";
    cout<<matrix(group1,group2)<<endl;
    cout<<"add: ("<<group2<<','<<group1<<")\n";
    cout<<matrix(group2,group1)<<endl;
#endif
    points.erase(points.begin()+index1);
    //NOTE,when index1<suitablePointsIndex[index2],the index of second point
    //changed as the erase of first point
    if(index1<suitablePointsIndex[index2])
      points.erase(points.begin()+suitablePointsIndex[index2]-1);
    else
      points.erase(points.begin()+suitablePointsIndex[index2]);
  }
  #ifdef debug
  if(matrix.nnz() != n*d)
    {
      cout<<matrix.rows()<<' '<<matrix.cols()<<endl;
      cout<<matrix;
      exit(1);
    }
  #endif
  return matrix;
}
inline int randint(int n){
  return rand()%n;
}
inline int group(int point,int d){
  return int(point/d);
}
template<typename T> 
void printVector(const vector<T> &vect){
  for(typename vector<T>::const_iterator iter=vect.begin();iter != vect.end();++iter)
    cout<<*iter<<' ';
  cout<<endl;
}
// regular.cpp ends here
