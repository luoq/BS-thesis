/* regular.h --- 
 * 
 * Filename: regular.h
 * Description: 
 * Author: Luo Qiang
 * Created: 03/15/2010 16:34:41
 * Version: 
 * Last-Updated: 03/15/2010 19:05:52
 *           By: Luo Qiang
 *     Update #: 25
 * Keywords: */

/* Commentary: */

/* Change log:*/

/* Code: */
#ifndef REGULAR_H
#define REGULAR_H

#include<iostream>
#include<vector>
#include<itpp/itbase.h>
using namespace	itpp;
using namespace	std;

Sparse_Mat<int> regular(int n,int d);
template<typename T>
void printMatrix(const Sparse_Mat<T> &matrix){
  for(int i=0;i<matrix.rows();i++){
    for(int j=0;j<matrix.cols();j++)
      cout<<matrix(i,j)<<' ';
    cout<<endl;
  }
}
template<typename T>
ostream & operator<<(ostream &out,const Sparse_Mat<T> &matrix){
  for(int i=0;i<matrix.rows();i++)
    for(int j=0;j<matrix.cols();j++)
      if(matrix(i,j) !=	0)
	out<<i+1<<'\t'<<j+1<<'\t'<<matrix(i,j)<<'\n';
  out<<matrix.rows()<<'\t'<<matrix.cols()<<'\t'<<0<<endl;
  return out;
}
// regular.cpp ends here
#endif
/* regular.h ends here */
