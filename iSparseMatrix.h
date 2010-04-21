/* iSparseMatrix.h ---
 *
 * Filename: iSparseMatrix.h
 * Description: my sparse matrix library
 * Author: Luo Qiang
 * Created: 03/17/2010 14:32:26
 * Version:
 * Last-Updated: 04/21/2010 09:38:30
 *           By: Luo Qiang
 *     Update #: 862
 * Keywords:
 */

// Commentary:
//use vector of sparse vector format to store
//considering the much add and erase in my work
//provide easy interface

// Change log:

// Code:
#ifndef ISPARSEMATRIX_H
#define ISPARSEMATRIX_H
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "misc.h"
#include "iFullMatrix.h"
using namespace std;
template<typename T> class	element;
template<typename T> bool operator<(const element<T> &,const element<T>&);
template<typename T> bool operator==(const element<T> &,const element<T>&);
template<typename T> class	svec;
template<typename T> ostream& operator<<(ostream&,const svec<T> &v);
template<typename T> class	smat;
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);



//eliminate minimal row
template<typename T> T	IDEM0(smat<T> &m,int node=0);
//eliminate minimal row and column
template<typename T> T	IDEM(smat<T> &m,int node=0);
//hybrd IDEM and RNW
template<typename T> T	H(smat<T> &m,int node=0);
//for 3-regular only
//eliminate row with <=2 elements
//then eliminate row intersecting with maxium column
template<typename T> T	DEM(smat<T> &m,int node=1);
//for 3-regular only
//change some recursion to iteration
template<typename T> T	DEMiter(smat<T> &m,int node=1);
//for 3-regular only
//eliminate row with <=2 elements
//then column <=1 elements
//then eliminate row intersecting with maxium column
template<typename T> T	DEM2(smat<T> &m,int node=1);
//for 3-regular only
//eliminate row with <=2 elements
//then column <=1 elements
//then row with minimal elements
template<typename T> T	IDEM3(smat<T> &m,int node=1);
//for 3-regular only
//hybrid IDEM3 and RNW
template<typename T> T	H3(smat<T> &m,int node=1);
template<typename T> void	peelDEM(smat<T> &m,bool& end,T& ret);

template<typename T> vector<int> selectElements(const smat<T>&,int& trytimes);
template<typename T> void eliminate2(smat<T> &m,int r,int c1,int c2,T value1,T value2);
template<typename T> void eliminate2T(smat<T> &m,int c,int r1,int r2,T value1,T value2);
template<typename T> void eliminate1(smat<T> &m,int r,int c);



template<typename T> class element
{
public:
  T	value;
  int	index;
  element(int index,T value)
    :value(value),index(index)
  {}
  element<T>&operator =	(const element<T>& other)
  {
    value = other.value;
    index = other.index;
    return *this;
  }
};
template<typename T> bool operator<(const element<T> &e1,const element<T> &e2)
{
  return e1.index<e2.index;
}
template<typename T> bool operator==(const element<T> &e1,const element<T> &e2)
{
  return e1.index==e2.index;
}

template <typename T> class svec
{
  template <typename U> friend class	smat;
  friend ostream& operator<<<> (ostream& Out,const svec<T>& v);
  friend ostream& operator<<<> (ostream& Out,const smat<T>& m);
  friend void eliminate2<>(smat<T> &m,int r,int c1,int c2,T value1,T value2);
  friend void eliminate2T<>(smat<T> &m,int c,int r1,int r2,T value1,T value2);
  friend T H<>(smat<T> &m,int node);
  friend T H3<>(smat<T> &m,int node);
  friend T IDEM<>(smat<T> &m,int node);
  friend T IDEM0<>(smat<T> &m,int node);
  friend T DEM<>(smat<T> &m,int node);
  friend T DEM2<>(smat<T> &m,int node);
  friend T IDEM3<>(smat<T> &m,int node);
  friend vector<int> selectElements<>(const smat<T>&,int& trytimes);
  friend void peelDEM<>(smat<T> &m,bool& end,T& ret);
  friend T DEMiter<>(smat<T> &m,int node);
public:
  svec (){_size	= 0;}
  svec(int size,int ennz);
  svec(int size)
    :_size(size)
  {}
  int size(){return _size;}
  int	nnz(){return data.size();}
  T operator()(int i) const;
  //return 1 for add,-1 for erase,0 for change or nothing
  int	set(int i,T value);
  //erase and return the value erased
  //return 0 for nothing to erase
  T erase(int i);
  void clear();

  //return sum of all elements
  T sum();
protected:
  vector<element<T> > data;
  unsigned		_size;
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
  if(targetIterator>=data.end())
    return 0;
  if((*targetIterator).index==i)
    return (*targetIterator).value;
  return 0;
}
template<typename T>
int svec<T>::set(int i,T value)
{
  element<T>	target(i,value);
  if(data.empty())
    {
      if(value==0)
	return 0;
      data.push_back(target);
      return 1;
    }
  if(i>=_size)
    {
      if(target.value!=0)
	{
	  _size=i+1;
	  data.push_back(target);
	  return 1;
	}
      return 0;
    }
  typename vector<element<T> >::iterator targetIterator = lower_bound(data.begin(),data.end(),target);
  if(targetIterator==data.end())
    {
      if(value==0)
	return 0;
      data.push_back(target);
      return 1;
    }
  if((*targetIterator).index==i)
    {
      if(value==0)
	{
	  data.erase(targetIterator);
	  return -1;
	}
      else
	{
	  (*targetIterator).value = value;
	  return 0;
	}
    }
  if(value==0)
    return 0;
  else
    {
      data.insert(targetIterator,target);
      return 1;
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
template<typename T>
void svec<T>::clear()
{
  data.clear();
}
template<typename T>
T svec<T>::sum()
{
  T	sum    = 0;
  for(int j=0;j<data.size();j++)
    sum += data[j].value;
  return sum;
}

template<typename T> class smat{
  friend ostream& operator<<<> (ostream& out,const smat<T>& m);
  friend void eliminate2<>(smat<T> &m,int r,int c1,int c2,T value1,T value2);
  friend void eliminate2T<>(smat<T> &m,int c,int r1,int r2,T value1,T value2);
  friend T H<>(smat<T> &m,int node);
  friend T H3<>(smat<T> &m,int node);
  friend T IDEM<>(smat<T> &m,int node);
  friend T IDEM0<>(smat<T> &m,int node);
  friend T DEM<>(smat<T> &m,int node);
  friend T DEM2<>(smat<T> &m,int node);
  friend T IDEM3<>(smat<T> &m,int node);
  //generate a perfect match
  friend vector<int> selectElements<>(const smat<T>&,int& trytimes);
  friend void peelDEM<>(smat<T> &m,bool& end,T& ret);
  friend T DEMiter<>(smat<T> &m,int node);
public:
  smat()
  {
#ifndef nonnz
    _nnz = 0;
#endif
    _cols=0;
  }
  smat(int rows,int cols)
    :_cols(cols),data(vector<svec<T> >(rows,svec<T>(cols)))
  {
#ifndef nonnz
    _nnz=0;
#endif
#ifdef colnnzs
    _col_nnzs.assign(_cols,0);
#endif
  }
  bool	load(char* path);
  //initialize with size and allocate at least eMaxCols for each row
  smat(int rows,int cols,int eMaxCols);
  //get the element at r,c
  T operator()(unsigned r,unsigned c) const;
  //set element at (r,c)
  void	set(int r,int c,T element);
  //earse element (r,c) and return its value
  T	erase(int r,int c);
  void	erase_row(unsigned r);
  void	erase_col(unsigned c);
  void	clear();
  //convert to a full matrix
  fmat<T> full() const;

  //find element in a range,signal error info if fail
  void find(int r,int c,int start,int end,int& pos,int& info)
  {
    element<T>	target(c,0);
    pos	= lower_bound_find(data[r].data.begin()+start,
			   data[r].data.begin()+end,
			   target,
			   info)-data[r].data.begin();
  }
  element<T>& int_element(int r,int i)
  {
    return data[r].data[i];
  }
  void int_erase(int r,int i)
  {
//#ifdef colnnzs
//	  _col_nnzs[int_element(r,i).index]--;
//#endif
	  data[r].data.erase(data[r].data.begin()+i);
  }
  void int_insert(int r,int i,element<T> e)
  {
//#ifdef colnnzs
//	  _col_nnzs[int_element(r,i).index]++;
//#endif
	  data[r].data.insert(data[r].data.begin()+i,e);
  }

  int rows() const {return data.size();}
  int cols() const {return _cols;}
  int nnz() const
  {
#ifndef nonnz
    return _nnz;
#else
    int nnz=0;
    for(int r=0;r<data.size();r++)
      nnz+=data[r].data.size();
    return nnz;
#endif
  }

  T	row_sum(int r) const;
  T	col_sum(int c) const;
  int	row_nnz(int r) const;
  int	col_nnz(int c) const;
  vector<int> col_nnzs() const;
  void print() const;
protected:
  unsigned _cols;
#ifndef nonnz
  unsigned _nnz;
#endif
#ifdef colnnzs
  vector<int> _col_nnzs;
#endif
  vector<svec<T> > data;
};
template<typename T>
smat<T>::smat(int rows,int cols,int eMaxCols)
 :_cols(cols)
{
#ifndef nonnz
  _nnz=0;
#endif
  data.reserve(rows);
  data.assign(rows,svec<T>(cols,eMaxCols));
#ifdef colnnzs
  _col_nnzs.assign(_cols,0);
#endif
}
template <typename T>
void smat<T>::set(int r,int c,T value)
{
#ifndef noautoenlarge
  //this following allows change of size,which is not  useful in my experiment
  if(r>=data.size())
    {
      if(value!=0)
	data.resize(r+1);
      else
	return;
    }
  if(c>=_cols)	//this test maybe wrong other than int type
    {
      if(value!=0)
	_cols = c+1;
      else
	return;
    }
#endif
  int temp=data[r].set(c,value);
#ifndef nonnz
  _nnz+=temp;
#endif
#ifdef colnnzs
  _col_nnzs[c]+=temp;
#endif
}
template<typename T>
T smat<T>::operator()(unsigned r,unsigned c) const
{
  //disabale check to speed up
  //if(r>=data.size())
  //	return 0;
  return (data[r])(c);
}

template<typename T>
T smat<T>::erase(int r,int c)
{
  //disable check for speed
  //if(r>=data.size())
  //  return 0;
  T	ret = (data[r]).erase(c);
#ifndef nonnz
  if(ret!=0)
    _nnz--;
#endif
#ifdef colnnzs
  if(ret!=0)
    _col_nnzs[c]--;
#endif
  return ret;
}
template<typename T>
void smat<T>::erase_row(unsigned r)
{
  //disable check for speed
  //if(r>=data.size())
  //	return;
#ifndef nonnz
  _nnz-=data[r].data.size();
#endif
#ifdef colnnzs
  for(int i=0;i<data[r].data.size();i++)
    _col_nnzs[data[r].data[i].index]--;
#endif
  data.erase(data.begin()+r);
}
template<typename T>
void smat<T>::erase_col(unsigned c)
{
  //disable check for speed
  //if(c>=_cols)
  //	return;
  element<T>	target(c,0);
  int		pos,info;
#ifndef nonnz
  int numErased=0;
#endif
  for(unsigned r=0;r<data.size();r++)
    {
      pos = lower_bound_find(data[r].data.begin(),data[r].data.end(),target,info)-data[r].data.begin();
      if(info==-1 || info==2)
	continue;
      for(unsigned col=pos;col<data[r].data.size();col++)
	data[r].data[col].index--;
      if(info==0)
	{
#ifndef nonnz
	  numErased++;
#endif
	  data[r].data.erase(data[r].data.begin()+pos);
	}
    }
#ifndef nonnz
  _nnz-=numErased;
#endif
#ifdef colnnzs
  _col_nnzs.erase(_col_nnzs.begin()+c);
#endif
  _cols--;
}

template<typename T>
ostream & operator<<(ostream &out,const smat<T> &m)
{
  out<<"# Created by iSpareMatrix"<<endl;
  out<<"# who am i"<<endl;
  out<<"# type: sparse matrix"<<endl;
  out<<"# nnz:"<<m.nnz()<<endl;
  out<<"# rows:"<<m.data.size()<<endl;
  out<<"# columns:"<<m._cols<<endl;
  for(int r=0;r<m.data.size();r++)
    for(int i=0;i<m.data[r].data.size();i++)
      out<<r<<'\t'<<(m.data[r].data)[i].index
	 <<'\t'<<(m.data[r].data)[i].value<<endl;
  return out;
}
template<typename T>
void smat<T>::clear()
{
#ifndef nonnz
  _nnz	= 0;
#endif
#ifdef colnnzs
  _col_nnzs.assign(_cols,0);
#endif
  for(int r=0;r<data.size();r++)
    data[r].clear();
}
template<typename T>
fmat<T> smat<T>::full() const
{
  fmat<T>	m(data.size(),_cols);
  for(int r=0;r<data.size();r++)
    for(int i=0;i<data[r].data.size();i++)
      m(r,data[r].data[i].index)=data[r].data[i].value;
  return m;
}
template<typename T>
T smat<T>::row_sum(int r) const
{
  return data[r].sum();
}
template<typename T>
T smat<T>::col_sum(int c) const
{
  T	sum  = 0;
  for(int r=0;r<data.size();r++)
    sum	    += data[r](c);
  return sum;
}
template<typename T>
int smat<T>::row_nnz(int r) const
{
  return data[r].data.size();
}
template<typename T>
int smat<T>::col_nnz(int c) const
{
#ifdef colnnzs
  return _col_nnzs[c];
#else
  int nnz  = 0;
  for(int r=0;r<data.size();r++)
    nnz+=int(data[r](c)!=0);
  return nnz;
#endif
}

template<typename T>
bool smat<T>::load(char* path)
{
  ifstream	In(path);
  if(!In)
    return false;

  //get head and parse
  string	head;
  int		minIndex   = 0;
  getline(In,head);
  if(head.find("Octave ") != string::npos)
    minIndex=1;

  //get matrix size and nnz
  int nnz,rows,cols;
  In.ignore(1024,'\n');
  In.ignore(1024,'\n');
  In.ignore(1024,':');
  In>>nnz;
  In.ignore(1024,':');
  In>>rows;
  In.ignore(1024,':');
  In>>cols;

  _cols = cols;
  data.assign(rows,svec<T>(cols,nnz/rows));
#ifdef colnnzs
  _col_nnzs.assign(cols,0);
#endif

  int	r,c;
  T	value;
  while(In>>r>>c>>value)
    {
      r	-= minIndex;
      c	-= minIndex;
      //data[r].data.push_back(element<T>(c,value));
      this->set(r,c,value);
    }
  return true;
}

template<typename T>
vector<int> smat<T>::col_nnzs() const
{
#ifdef colnnzs
  return _col_nnzs;
#else
  //get column with minimal elements
  vector<int>	helpIndex(data.size(),-1);
  vector<int>	colSize(_cols,0);
  for(int r=0;r<helpIndex.size();r++)
    if(!data[r].data.empty())
      helpIndex[r] = 0;
  for(int r=0;r<data.size();r++)
    {
      for(int c=0;c<_cols;c++)
	{
	  if(helpIndex[r]==-1)
	    break;
	  if(c==data[r].data[helpIndex[r]].index)
	    {
	      colSize[c]++;
	      if(helpIndex[r]==data[r].data.size()-1)
		helpIndex[r] = -1;
	      else
		helpIndex[r]++;
	    }
	}
    }
  return colSize;
#endif
}
template <typename T>
void smat<T>::print() const
{
  //cout<<100*(double)_nnz/(_cols*_cols)<<" %\\n";
#ifdef plot
#ifndef nonnz
  cout<<_nnz<<"\\n";
#endif
  for(int i=0;i<data.size();i++)
    {
      for(int j=0;j<_cols;j++)
	cout<<this->operator()(i,j)<<' ';
      cout<<"\\n";
    }
#else
  for(int i=0;i<data.size();i++)
    {
      for(int j=0;j<_cols;j++)
	cout<<this->operator()(i,j)<<' ';
      cout<<"\n";
    }
#endif
}
#endif /* _ISPARSEMATRIX_H_ */
