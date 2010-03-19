/* iSparseMatrix.h ---
 *
 * Filename: iSparseMatrix.h
 * Description: my sparse matrix library
 * Author: Luo Qiang
 * Created: 03/17/2010 14:32:26
 * Version:
 * Last-Updated: 03/19/2010 17:12:39
 *           By: Luo Qiang
 *     Update #: 213
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

template<typename T>class	element;
template<typename T> bool operator<(const element<T> &,const element<T>&);
template<typename T>class	svec;
template<typename T> ostream& operator<<(ostream&,const svec<T> &m);
template<typename T>class	smat;
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);

template<typename T> class element
{
public:
  T	value;
  int	index;
  element(int index,T value)
    :value(value),index(index)
  {}
};
template<typename T> bool operator<(const element<T> &e1,const element<T> &e2)
{
  return e1.index<e2.index;
}
template <typename T> class svec
{
  friend class smat<T>;
  friend ostream& operator<<<> (ostream& out,const svec<T>& m);
public:
  svec(int size,int ennz);
  int size(){return _size;}
  int nnz(){return data.size();}
  T operator()(int i) const;
  void set(int i,T value);
  T erase(int i);
protected:
  vector<element<T> > data;
  int		_size;
};
template<typename T>
svec<T>::svec(int size,int ennz)
  :_size(size)
{
  data.reserve(ennz);
}
template<typename T>
T svec<T>::operator()(int i) const
{
  if(data.empty())
    return 0;
  element<T> target(i,0);
  typename vector<element<T> >::const_iterator targetIterator
    =lower_bound(data.begin(),data.end(),target);
  if(targetIterator==data.end())
    return 0;
  if((*targetIterator).index==i)
    return (*targetIterator).value;
  return 0;
}
template<typename T>
void svec<T>::set(int i,T value)
{
  element<T>	target(i,value);
  if(data.empty())
    {
      if(value==0)
	return;
      data.push_back(target);
      return;
    }
  if(i>=_size)
    {
      if(target.value!=0)
	{
	  data.push_back(target);
	  return;
	}
      return;
    }
  typename vector<element<T> >::iterator targetIterator = lower_bound(data.begin(),data.end(),target);
  if(targetIterator==data.end())
    {
      if(value==0)
	return;
      data.push_back(target);
      return;
    }
  if((*targetIterator).index==i)
    {
      if(value==0)
	{
	  data.erase(targetIterator);
	  return;
	}
      else
	{
	  (*targetIterator).value = value;
	  return;
	}
    }
  if(value==0)
    return;
  else
    {
      data.insert(targetIterator,target);
      return;
    }
}
template<typename T>
T svec<T>::erase(int i)
{
  if(data.empty())
    return 0;
  element<T> target(i,0);
  typename vector<element<T> >::iterator	targetIterator = lower_bound(data.begin(),data.end(),target);
  if(targetIterator==data.end())
    return 0;
  if((*targetIterator).index==i)
    {
      T temp=(*targetIterator).value;
      data.erase(targetIterator);
      return temp;
    }
  return 0;
}
template<typename T>
ostream& operator<< (ostream& out,const svec<T>& v)
{
  out<<"# size: "<<v._size<<endl;
  out<<"# nnz: "<<v.data.size()<<endl;
  for(typename vector<element<T> >::const_iterator i=v.data.begin();i != v.data.end();++i)
    out<<(*i).index<<'\t'<<(*i).value<<' '<<endl;
  return out;
}


template<typename T> class smat{
  friend ostream& operator<<<> (ostream& out,const smat<T>& m);
public:
  smat(){}
  //initialize with size and allocate at least nnz for nonzero elements
  smat(int rows,int cols,int nnz);
  //initializer with size and all the nonzero elements
  smat(int rows,int colums,vector<T> elements,vector<T> rowIndex,vector<T> colIndex);
  //get the element at r,c
  T operator()(int r,int c) const;
  //set element at (r,c)
  void	set(int r,int c,T element);
  //earse element (r,c) and return its value
  T erase(int r,int c);
  void clear();

  int rows() const {return rowPointers.size();}
  int cols() const {return _cols;}
  int nnz() const {return elements.size();}
protected:
  int	_cols;
  vector<T>	elements;
  vector<T>	columns;
  vector<T>	rowPointers;
};

template<typename T>
smat<T>::smat(int rows,int cols,int nnz)
  :_cols(cols),rowPointers(vector<int>(rows,0))
{
  elements.reserve(nnz);
  columns.reserve(nnz);
}
template<typename T>
smat<T>::smat(int rows,int colums,vector<T> elements,vector<T> rowIndex,vector<T> colIndex)
{
}
template <typename T>
void smat<T>::set(int r,int c,T element)
{
  //this following allows change of size,which is not  useful in my experiment
  if(r>=rowPointers.size() && element!=0)	//this test maybe wrong other than int type
    {
      rowPointers.resize(r+1,elements.size());
    }
  if(c>=_cols && element!=0)	//this test maybe wrong other than int type
    _cols = c+1;

  vector<int>::iterator begin,end,target;
  begin					   = columns.begin()+rowPointers[r];
  //if rowPointers[r] exceed the size of colums
  //In this program it's only possible when
  //begin == colums.end()
  //we will push an element in,if element != 0
  if(begin>=columns.end())
    {
      if(element!=0)
	{
	  columns.push_back(c);
	  elements.push_back(element);
	  for(int i=r+1;i<rowPointers.size();++i)
	    ++rowPointers[i];
	}
      return;
    }
  //find the end of column c
  //note we begin with index 0
  if(r==rowPointers.size()-1)
    end	      = columns.end();
  else
    end	      = columns.begin()+rowPointers[r+1];
  //if begin ==	end there is no element in that row
  //but lower_bound will return begin,
  //which point to a place in columns,valid to insert
  //and we force it not check column,just insert.
  if(begin==end)
    {
      if(element!=0)
	{
	  elements.insert(elements.begin()+(begin-columns.begin()),element);
	  columns.insert(begin,c);
	  for(int i=r+1;i<rowPointers.size();++i)
	    ++rowPointers[i];
	}
      return;
    }
  target = lower_bound(begin,end,c);
  //we will add element to the last of elemets
  //in case columns.end() has some random number equals to c
  if(target==columns.end())
    {
      if(element!=0)
	{
	  columns.push_back(c);
	  elements.push_back(element);
	  for(int i=r+1;i<rowPointers.size();++i)
	    ++rowPointers[i];
	}
      return;
    }
  //already an element exsits
  if(*target==c)
    {
      //if assign 0,then erase the element
      //this only works in int matrix
      if(element==0)
	{
	  elements.erase(elements.begin()+(target-columns.begin()));
	  columns.erase(target);
	  for(int i=r+1;i<rowPointers.size();++i)
	    --rowPointers[i];
	  return;
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
      for(int i=r+1;i<rowPointers.size();++i)
	++rowPointers[i];
    }
  return;
}
template<typename T>
T smat<T>::operator()(int r,int c) const
{
  //NOTE:we need const_iterator here

  vector<int>::const_iterator	begin,end,target;
  begin	      = columns.begin()+rowPointers[r];
  if(r==rowPointers.size()-1)
    end	      = columns.end();
  else
    end	      = columns.begin()+rowPointers[r+1];
  //if begin ==	end there is no element in that row
  //but lower_bound will return begin,causing error
  if(begin==end)
    return 0;
  target      = lower_bound(begin,end,c);
  //in case influence of random data at columns.end()
  if(target>=columns.end())
    return 0;
  if(*target==c)
    return *(elements.begin()+(target-columns.begin()));
  return 0;
}
template<typename T>
T smat<T>::erase(int r,int c)
{
  vector<int>::iterator	begin,end,target;
  begin	      = columns.begin()+rowPointers[r];
  if(r==rowPointers.size()-1)
    end	      = columns.end();
  else
    end	      = columns.begin()+rowPointers[r+1];
  //if begin ==	end there is no element in that row
  //but lower_bound will return begin,causing error
  //just do nothing
  if(begin==end)
    return 0;
  target = lower_bound(begin,end,c);
  //in case influence of random data at columns.end()
  if(target>=columns.end())
    return 0;
  if(*target==c)
    {
      T temp=*(elements.begin()+(target-columns.begin()));
      elements.erase(elements.begin()+(target-columns.begin()));
      columns.erase(target);
      for(int i=r+1;i<rowPointers.size();++i)
	--rowPointers[i];
      return temp;
    }
  return 0;
}

template<typename T>
ostream & operator<<(ostream &out,const smat<T> &m)
{
  out<<"# nnz :"<<m.elements.size()<<endl;
  out<<"# rows :"<<m.rowPointers.size()<<endl;
  out<<"# colums :"<<m._cols<<endl;
  for(int i=0;i<m.rowPointers.size()-1;++i)
    for(int j=m.rowPointers[i];j<m.rowPointers[i+1];++j)
      out<<i<<' '<<m.columns[j]<<' '<<m.elements[j]<<endl;
  //note the situation at the last row where rowPointers[rows] is invalid
  for(int j=m.rowPointers[m.rowPointers.size()-1];j<m.elements.size();++j)
    out<<m.rowPointers.size()-1<<' '<<m.columns[j]<<' '<<m.elements[j]<<endl;
  return out;
}
template<typename T>
void smat<T>::clear()
{
  elements.clear();
  columns.clear();
  rowPointers.assign(rowPointers.size(),0);
}

#endif
/* iSparseMatrix.h ends here */
