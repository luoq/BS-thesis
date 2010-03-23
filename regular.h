/* regular.h --- 
 * 
 * Filename: regular.h
 * Description: 
 * Author: Luo Qiang
 * Created: 03/15/2010 16:34:41
 * Version: 
 * Last-Updated: 03/23/2010 11:14:16
 *           By: Luo Qiang
 *     Update #: 48
 * Keywords: */

/* Commentary: */

/* Change log:*/

/* Code: */
#ifndef REGULAR_H
#define REGULAR_H

#include<iostream>
#include<vector>
#include "iSparseMatrix.h"
#include "misc.h"
using namespace	std;
//#define debug

smat<int>	regular(int n,int d,int& trytimes);
template<typename T> bool isregular(const smat<T> &m,int d);

template<typename T>
bool isregular(const smat<T> &m,int d)
{
  for(int r=0;r<m.rows();r++)
    if(m.row_nnz(r) != d)
      {
#ifdef debug
	cout<<"failed at row:"<<r<<endl;
#endif
	return false;
      }
  for(int c=0;c<m.cols();c++)
    if(m.col_nnz(c) != d)
      {
#ifdef debug
	cout<<"failed at col:"<<c<<endl;
#endif
	return false;
      }
  return true;
}

template<typename T>
void printMatrix(const smat<T> &matrix){
  for(int i=0;i<matrix.rows();i++){
    for(int j=0;j<matrix.cols();j++)
      cout<<matrix(i,j)<<' ';
    cout<<endl;
  }
}
#endif
/* regular.h ends here */
