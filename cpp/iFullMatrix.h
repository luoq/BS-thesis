/* iFullMatrix.h --- 
 * 
 * Filename: iFullMatrix.h
 * Description: full matrix
 * Author: Luo Qiang
 * Created: 03/27/2010 15:04:15
 * Version: 
 * Last-Updated: 04/21/2010 09:03:50
 *           By: Luo Qiang
 *     Update #: 27
 * Keywords: 

* Commentary: 

* Change log:

* Code: */
#ifndef IFULLMATRIX_H
#define IFULLMATRIX_H

#include<vector>
#include<iostream>
using namespace std;

template<typename T>class	fmat;
template<typename T>ostream& operator<<(ostream& out,const fmat<T> &fm);

template<typename T> class fmat
{
public:
  fmat(int rows,int cols);
  fmat(){}
  
  int rows ()const {return _rows;}
  int cols ()const {return _cols;}
  T&		operator()(int r,int c);
  const T&	operator()(int r,int c) const;
 private:
  vector<T>	data;
  int		_rows;
  int		_cols;
};

template<typename T>
fmat<T>::fmat(int rows,int cols)
  :_rows(rows),_cols(cols)
{
  data.reserve(_rows*_cols);
  data.assign(_rows*_cols,0);
}
template<typename T>
T& fmat<T>::operator()(int r,int c)
{
  return data[r*_rows+c];
}
template<typename T>
const T& fmat<T>::operator()(int r,int c) const
{
  return data[r*_rows+c];
}
template<typename T>
ostream& operator<<(ostream& out,const fmat<T> &m)
{
  for(int r=0;r<m.rows();r++)
    {
      for(int c=0;c<m.cols();c++)
	out<<m(r,c)<<'\t';
      out<<endl;
    }
  return out;
}

#endif
/* iFullMatrix.h ends here */
