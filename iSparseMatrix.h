/* iSparseMatrix.h ---
 *
 * Filename: iSparseMatrix.h
 * Description: my sparse matrix library
 * Author: Luo Qiang
 * Created: 03/17/2010 14:32:26
 * Version:
 * Last-Updated: 03/17/2010 23:02:54
 *           By: Luo Qiang
 *     Update #: 95
 * Keywords:

 /* Commentary:
 //use CSR format to store
 //provide easy interface

 /* Change log:

 /* Code: */
#ifndef ISPARSEMATRIX_H
#define ISPARSEMATRIX_H
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template<typename T>class smat;
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);

template<typename T> class smat{
public:
  //initialize with size and allocate at least nnz for nonzero elements
  smat(int rows,int cols,int nnz);
  //initializer with size and all the nonzero elements
  smat(int rows,int colums,vector<T> elements,vector<T> rowIndex,vector<T> colIndex);
  //get the element at r,c
  T operator()(int r,int c) const;
  //set element at (r,c)
  void clear();
  void	set(int r,int c,T element);
  int rows() const {return _rows;}
  int cols() const {return _cols;}
  int nnz() const {return elements.size();}
protected:
  int	_rows;
  int	_cols;
  vector<T>	elements;
  vector<T>	columns;
  vector<T>	rowPointers;
  friend ostream& operator<<<> (ostream& out,const smat<T>& m);
};

template<typename T>
smat<T>::smat(int rows,int cols,int nnz)
  :_rows(rows),_cols(cols),rowPointers(vector<int>(rows,0))
{
  elements.reserve(nnz);
  columns.reserve(nnz);
}
template<typename T>
smat<T>::smat(int rows,int colums,vector<T> elements,vector<T> rowIndex,vector<T> colIndex)
{
}
template<typename T>
void smat<T>::set(int r,int c,T element)
{
  //this following allows change of size,which is not  useful in my experiment
  if(r>=_rows && element!=0)	//this test maybe wrong other than int type
    {
      rowPointers.resize(r+1,elements.size());
      _rows = r+1;
    }
  if(c>=_cols && element!=0)	//this test maybe wrong other than int type
    _cols = c+1;

  vector<int>::iterator begin,end,target;
  begin = columns.begin()+rowPointers[r];
  //note we begin with index 0
  if(r==_rows-1)
    end	 = columns.end();
  else
    end	= columns.begin()+rowPointers[r+1];
  target = lower_bound(begin,end,c);
  if(*target==c)
    {
      //if assign 0,then do nothing
      //this only works in int matrix
      if(element==0)
	{
	  elements.erase(elements.begin()+(target-columns.begin()));
	  columns.erase(target);
	  for(int i=r+1;i<_rows;++i)
	    --rowPointers[i];
	}
      *(elements.begin()+(target-columns.begin())) = element;
    }
  else
    {
      //if assign 0,then do nothing
      //this only works in int matrix
      if(element==0)
	return;
      elements.insert(elements.begin()+(target-columns.begin()),element);
      columns.insert(target,c);
      for(int i=r+1;i<_rows;++i)
	++rowPointers[i];
    }
}
template<typename T>
T smat<T>::operator()(int r,int c) const
{
  //NOTE:we need const_iterator here 
  vector<int>::const_iterator begin,end,target;
  begin = columns.begin()+rowPointers[r];
  if(r==_rows-1)
    end	 = columns.end();
  else
    end	= columns.begin()+rowPointers[r+1];
  target = lower_bound(begin,end,c);
  if(*target==c)
      return *(elements.begin()+(target-columns.begin()));
  else
    return 0;
}

template<typename T>
ostream & operator<<(ostream &out,const smat<T> &m)
{
  out<<"# nnz :"<<m.elements.size()<<endl;
  out<<"# rows :"<<m._rows<<endl;
  out<<"# colums :"<<m._cols<<endl;
  for(int i=0;i<m._rows-1;++i)
    for(int j=m.rowPointers[i];j<m.rowPointers[i+1];++j)
      out<<i<<' '<<m.columns[j]<<' '<<m.elements[j]<<endl;
  //note the situation at the last row where rowPointers[rows] is invalid
  for(int j=m.rowPointers[m._rows-1];j<m.elements.size();++j)
    out<<m._rows-1<<' '<<m.columns[j]<<' '<<m.elements[j]<<endl;
  return out;
}
template<typename T>
void smat<T>::clear()
{
  elements.clear();
  columns.clear();
  rowPointers.assign(_rows,0);
}

#endif
/* iSparseMatrix.h ends here */
