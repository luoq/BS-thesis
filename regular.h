/* regular.h --- 
 * 
 * Filename: regular.h
 * Description: 
 * Author: Luo Qiang
 * Created: 03/15/2010 16:34:41
 * Version: 
 * Last-Updated: 03/18/2010 10:55:08
 *           By: Luo Qiang
 *     Update #: 34
 * Keywords: */

/* Commentary: */

/* Change log:*/

/* Code: */
#ifndef REGULAR_H
#define REGULAR_H

#include<iostream>
#include<vector>
#include "iSparseMatrix.h"
using namespace	std;


smat<int> regular(int n,int d);
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
