/* iSparseMatrix.h ---
 *
 * Filename: iSparseMatrix.h
 * Description: my sparse matrix library
 * Author: Luo Qiang
 * Created: 03/17/2010 14:32:26
 * Version:
 * Last-Updated: 03/22/2010 10:43:48
 *           By: Luo Qiang
 *     Update #: 637
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

using namespace std;

template<typename T> class	element;
template<typename T> bool operator<(const element<T> &,const element<T>&);
template<typename T> bool operator==(const element<T> &,const element<T>&);
template<typename T> class	svec;
template<typename T> ostream& operator<<(ostream&,const svec<T> &v);
template<typename T> class	smat;
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);
template<typename T> ostream& operator<<(ostream&,const smat<T> &m);
template<typename T> T		HPerm(smat<T> &m,int node=0);
//info = -1,empty
//info = 0,find
//info = 1,not find,return first larger
//info = 2,not find,at end
template <class			ForwardIterator, class T>
ForwardIterator lower_bound_find(ForwardIterator first, ForwardIterator last,
				 const T& value,int & info);


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
public:
  template <typename U> friend class	smat;
  friend ostream& operator<<<> (ostream& Out,const svec<T>& v);
  friend ostream& operator<<<> (ostream& Out,const smat<T>& m);
  friend T HPerm<>(smat<T> &m,int node);
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
  friend T HPerm<>(smat<T> &m,int node);
public:
  smat()
  {
    _nnz = 0;
    _cols=0;
  }
  smat(int rows,int cols)
    :_cols(cols),data(vector<svec<T> >(rows,svec<T>(cols))),_nnz(0)
  {}
  bool load(char* path);
  //initialize with size and allocate at least eMaxCols for each row
  smat(int rows,int cols,int eMaxCols);
  //get the element at r,c
  T operator()(unsigned r,unsigned c) const;
  //set element at (r,c)
  void	set(int r,int c,T element);
  //earse element (r,c) and return its value
  T erase(int r,int c);
  void erase_row(unsigned r);
  void erase_col(unsigned c);
  void clear();

  int	rows() const {return data.size();}
  int cols() const {return _cols;}
  int nnz() const {return _nnz;}

  T	row_sum(int r) const;
  T	col_sum(int c) const;
  int	row_nnz(int r) const;
  int	col_nnz(int c) const;
  void print() const;
protected:
  unsigned _cols;
  unsigned _nnz;
  vector<svec<T> > data;
};
template<typename T>
smat<T>::smat(int rows,int cols,int eMaxCols)
 :_nnz(0),_cols(cols),data(vector<svec<T> >(rows,svec<T>(cols,eMaxCols)))
{}
template <typename T>
void smat<T>::set(int r,int c,T value)
{
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
  _nnz+=(data[r]).set(c,value);
}
template<typename T>
T smat<T>::operator()(unsigned r,unsigned c) const
{
  if(r>=data.size())
    return 0;
  return (data[r])(c);
}

template<typename T>
T smat<T>::erase(int r,int c)
{
  if(r>=data.size())
    return 0;
  T	ret = (data[r]).erase(c);
  if(ret!=0)
    _nnz--;
  return ret;
}
template<typename T>
void smat<T>::erase_row(unsigned r)
{
  if(r>=data.size())
    return;
  _nnz-=data[r].data.size();
  data.erase(data.begin()+r);
}
template<typename T>
void smat<T>::erase_col(unsigned c)
{
  if(c>=_cols)
    return;
  element<T>	target(c,0);
  int		pos,info;
  int numErased=0;
  for(unsigned r=0;r<data.size();r++)
    {
      pos = lower_bound_find(data[r].data.begin(),data[r].data.end(),target,info)-data[r].data.begin();
      if(info==-1 || info==2)
	continue;
      for(unsigned col=pos;col<data[r].data.size();col++)
	data[r].data[col].index--;
      if(info==0)
	{
	  numErased++;
	  data[r].data.erase(data[r].data.begin()+pos);
	}
    }
  _nnz-=numErased;
  _cols--;
}

template<typename T>
ostream & operator<<(ostream &out,const smat<T> &m)
{
  out<<"# Created by iSpareMatrix"<<endl;
  out<<"# who am i"<<endl;
  out<<"# type: sparse matrix"<<endl;
  out<<"# nnz:"<<m._nnz<<endl;
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
  _nnz	= 0;
  for(int r=0;r<data.size();r++)
    data[r].clear();
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
  int nnz  = 0;
  for(int r=0;r<data.size();r++)
    nnz+=int(data[r](c)!=0);
  return nnz;
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

  _cols	= cols;
  _nnz	= nnz;
  //3 is for later calculation
  data.assign(rows,svec<T>(cols,nnz/rows*3));

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
T HPerm(smat<T> &m,int node=0)
{
  int max = 5;
  int child=1;
#ifdef plot
  cout<<"\""<<node<<"\"[label=\"node: "<<node<<"\\n";
  m.print();
  cout<<"\"];\n";
#endif
  if(m.data.size() == 1)
    {
#ifdef plot
      cerr<<"node"<<node<<" return :"<<m(0,0)<<endl;
#endif
      return m(0,0);
    }
  //find the row with minimal element
  vector<int>	rowSize(m.data.size());
  for(unsigned r=0;r<rowSize.size();r++)
    rowSize[r]=m.data[r].data.size();
  int	minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
  int	minSize	= rowSize[minRow];
#ifdef plot
  cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

  if(minSize==0)
    {
      return 0;
    }

  T ret=0;
  for(int i=0;i<minSize/2;i++)
    {
      int	c1	= m.data[minRow].data[0].index;
      T		value1	= m.data[minRow].data[0].value;
      int	c2	= m.data[minRow].data[1].index;
      T		value2	= m.data[minRow].data[1].value;
      m.data[minRow].data.erase(m.data[minRow].data.begin()+1);
      m.data[minRow].data.erase(m.data[minRow].data.begin());
      m._nnz	       -= 2;

      smat<T> mtemp(m);
      mtemp.erase_row(minRow);
      element<T> target1(c1,0),target2(c2,0);
      int		info1,info2;
      int		pos1,pos2;
      for(unsigned r=0;r<mtemp.data.size();r++)
	//eliminate two elements each row
	{
	  pos1 = lower_bound_find(mtemp.data[r].data.begin(),mtemp.data[r].data.end(),target1,info1)
	    -mtemp.data[r].data.begin();
	  if(info1==-1 || info1==2)
	    continue;
	  pos2 = lower_bound_find(mtemp.data[r].data.begin()+pos1,mtemp.data[r].data.end(),target2,info2)-mtemp.data[r].data.begin();
#ifdef plot
	  cerr<<"node"<<node<<" row: "<<r<<" pos1,pos2 = "<<pos1<<" , "<<pos2<<endl;
	  cerr<<"node"<<node<<" row: "<<r<<" inf1,inf2 = "<<info1<<" , "<<info2<<endl;
#endif
	  for(unsigned c=pos2;c<mtemp.data[r].data.size();c++)
	    mtemp.data[r].data[c].index--;
	  if(info1==0)
	    {
	      if(info2!=0)
		mtemp.data[r].data[pos1].value = value2*mtemp.data[r].data[pos1].value;
	      else
		{
		  T	tempValue =	value2*mtemp.data[r].data[pos1].value+value1*mtemp.data[r].data[pos2].value;
		  if(tempValue==0)
		    continue;
		  mtemp.data[r].data[pos1].value  =tempValue;
		  mtemp.data[r].data.erase(mtemp.data[r].data.begin()+pos2);
		  mtemp._nnz--;
		}
	      continue;
	    }
	  if(info1==1)
	    {
	      if(info2==0)
		{
		  //value1!=0
		  mtemp.data[r].data.insert(mtemp.data[r].data.begin()+pos1,
					    element<T>(c1,value1*mtemp.data[r].data[pos2].value));
		  //NOTE erase pos2+1 becuase the insert before
		  mtemp.data[r].data.erase(mtemp.data[r].data.begin()+pos2+1);
		}
	      continue;
	    }
	}
      mtemp._cols--;
#ifdef plot
      cerr<<"node"<<node<<" eliminate col :"<<c1<<'\t'<<c2<<endl;
      cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
      ret += HPerm(mtemp,node*max+child);
#ifdef plot
      child++;
#endif
    }
  if(!m.data[minRow].data.empty())
    {
      int c= m.data[minRow].data[0].index;
      T value= m.data[minRow].data[0].value;
#ifdef debug
  cout<<"will eliminate columns: "<<c<<endl;
#endif
      m.erase_row(minRow);
      m.erase_col(c);
#ifdef plot
      cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
      cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
      ret+=value*HPerm(m,node*max+child);
    }
#ifdef plot
      cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
  return ret;
}


template <class ForwardIterator, class T>
ForwardIterator lower_bound_find ( ForwardIterator first, ForwardIterator last,
				   const T& value,int & info)
{
  if(first>=last)
    {
      info = -1;
      return first;
    }
  ForwardIterator	ret = lower_bound(first,last,value);
  if(ret>=last)
    info = 2;
  else if(*ret==value)
    info = 0;
  else
    info = 1;
  return ret;
}
template <typename T>
void smat<T>::print() const
{
  for(int i=0;i<data.size();i++)
    {
      for(int j=0;j<_cols;j++)
	cout<<this->operator()(i,j)<<' ';
      cout<<"\\n";
    }
}
#endif
/* iSparseMatrix.h ends here */
