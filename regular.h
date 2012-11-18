/* regular.h --- 
 * 
 * Filename: regular.h
 * Description: 
 * Author: Luo Qiang
 * Created: 03/15/2010 16:34:41
 * Version: 
 * Last-Updated: 06/14/2010 09:45:21
 *           By: Luo Qiang
 *     Update #: 63
 * Keywords: */

/* Commentary: */

/* Change log:*/

/* Code: */
#ifndef REGULAR_H
#define REGULAR_H

#include<iostream>
#include<vector>
#include"iSparseMatrix.h"
using namespace	std;

smat<int>	regular(int n,int d);
smat<int>	regular2(int n,int d,int& trytimes);
//n:size,k:num of d,other is d-1
smat<int>	mix_regular(int n,int k,int d,int& trytimes);
smat<int> gen_with_nnzs0(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes);
smat<int> gen_with_nnzs(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes);
smat<int> gen_with_nnzs2(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes);
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
