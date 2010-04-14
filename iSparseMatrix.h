/* iSparseMatrix.h ---
 *
 * Filename: iSparseMatrix.h
 * Description: my sparse matrix library
 * Author: Luo Qiang
 * Created: 03/17/2010 14:32:26
 * Version:
 * Last-Updated: 04/10/2010 11:25:59
 *           By: Luo Qiang
 *     Update #: 828
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
#include "R-NW.h"
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
template<typename T> vector<int> aPerfectMatch(const smat<T>&,int& trytimes);
template<typename T> void eliminate2(smat<T> &m,int r,int c1,int c2,T value1,T value2);
template<typename T> void eliminate2T(smat<T> &m,int c,int r1,int r2,T value1,T value2);
template<typename T> void eliminate1(smat<T> &m,int r,int c);
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
	friend vector<int> aPerfectMatch<>(const smat<T>&,int& trytimes);
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
	friend vector<int> aPerfectMatch<>(const smat<T>&,int& trytimes);
	friend void peelDEM<>(smat<T> &m,bool& end,T& ret);
	friend T DEMiter<>(smat<T> &m,int node);
	public:
	smat()
	{
		_nnz = 0;
		_cols=0;
	}
	smat(int rows,int cols)
		:_cols(cols),data(vector<svec<T> >(rows,svec<T>(cols))),_nnz(0)
	{}
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

	int rows() const {return data.size();}
	int cols() const {return _cols;}
	int nnz() const {return _nnz;}


	T	row_sum(int r) const;
	T	col_sum(int c) const;
	int	row_nnz(int r) const;
	int	col_nnz(int c) const;
	vector<int> col_nnzs() const;
	void print() const;
	protected:
	unsigned _cols;
	unsigned _nnz;
	vector<svec<T> > data;
};
	template<typename T>
	smat<T>::smat(int rows,int cols,int eMaxCols)
:_nnz(0),_cols(cols)
{
	data.reserve(rows);
	data.assign(rows,svec<T>(cols,eMaxCols));
}
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
	//3 is for later calculation
	data.assign(rows,svec<T>(cols,nnz/rows));

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
}

const int changePoint=7;
	template<typename T>
T H(smat<T> &m,int node=0)
{
	int	max   = 10;
	int	child =	1;

	if(m.data.size() <= changePoint)
	{
#ifdef plot
		cout<<"\""<<node<<"\"[label=\"";
		//m.print();
		cout<<"end";
		cout<<"\",fillcolor=red];\n";
#endif
		return RNW(m.full());
	}

#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif
	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int		minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int		minRowSize	= rowSize[minRow];

	vector<int>	colSize	   = m.col_nnzs();
	int	minCol	   = min_element(colSize.begin(),colSize.end())-colSize.begin();
	int	minColSize = colSize[minCol];
#ifdef stat
	cerr<<(minRowSize<=minColSize?minRowSize:minColSize)<<endl;
#endif


	if(minRowSize<=minColSize)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif
		if(minRowSize==0)
			return 0;

#ifdef plot
		cout<<"\""<<node<<"\"[label=\"";
		//m.print();
		cout<<minRowSize;
		cout<<"\"];\n";
#endif

		T ret=0;
		for(int i=0;i<minRowSize/2;i++)
		{
			int	c1	= m.data[minRow].data[0].index;
			T		value1	= m.data[minRow].data[0].value;
			int	c2	= m.data[minRow].data[1].index;
			T		value2	= m.data[minRow].data[1].value;
			m.data[minRow].data.erase(m.data[minRow].data.begin()+1);
			m.data[minRow].data.erase(m.data[minRow].data.begin());
			m._nnz	       -= 2;

			{
				smat<T> mtemp(m);
				eliminate2(mtemp,minRow,c1,c2,value1,value2);
#ifdef plot
				cerr<<"node"<<node<<" eliminate col :"<<c1<<'\t'<<c2<<endl;
				cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
				ret += H(mtemp,node*max+child);
			}
#ifdef plot
			child++;
#endif
		}
		if(!m.data[minRow].data.empty())
		{
			int c= m.data[minRow].data[0].index;
			T value= m.data[minRow].data[0].value;
			eliminate1(m,minRow,c);
#ifdef plot
			cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
			cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
			ret+=value*H(m,node*max+child);
		}
#ifdef plot
		cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
		return ret;
	}
	else
	{
#ifdef plot
		cerr<<"node"<<node<<" min col :"<<minCol<<endl;
#endif
		if(minColSize==0)
			return 0;

#ifdef plot
		cout<<"\""<<node<<"\"[label=\"";
		//m.print();
		cout<<minColSize;
		cout<<"\",fillcolor=green];\n";
#endif

		T ret=0;
		vector<int> rows;
		vector<T> values;
		rows.reserve(minColSize);
		values.reserve(minColSize);
		for(unsigned r=0;rows.size()<minColSize;r++)
		{
			T temp=m(r,minCol);
			if(temp!=0)
			{
				rows.push_back(r);
				values.push_back(temp);
			}
		}
		for(unsigned i=0;i<minColSize-1;i+=2)
		{
			int	r1	= rows[i];
			T	value1	= values[i];
			int	r2	= rows[i+1];
			T	value2	= values[i+1];

			{
				smat<T> mtemp(m);
				eliminate2T(mtemp,minCol,r1,r2,value1,value2);
#ifdef plot
				cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
				ret += H(mtemp,node*max+child);
			}
#ifdef plot
				child++;
#endif
		}
		if(minColSize%2==1)
		{
			int r=rows.back();
			T value=values.back();
			eliminate1(m,r,minCol);
#ifdef plot
			cerr<<"node"<<node<<" eliminate row :"<<r<<endl;
			cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
			ret+=value*H(m,node*max+child);
		}
#ifdef plot
		cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
		return ret;
	}
}

	template<typename T>
T IDEM(smat<T> &m,int node=0)
{
	int	max   = 3;
	int	child =	1;
#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif

#ifdef plot
	cout<<"\""<<node<<"\"[label=\"node: "<<node<<"\\n";
	m.print();
	cout<<"\"];\n";
#endif


//	if(m.data.size() == 1)
//		return m(0,0);
	if(m.data.size() == 3)
	{
#ifdef plot
		cerr<<"node"<<node<<" return :"<<m(0,0)<<endl;
#endif
#ifdef stat
		cerr<<m.data.size()<<endl;
#endif
		return 
			m(0,0)*(m(1,1)*m(2,2)+m(1,2)*m(2,1))+
			m(0,1)*(m(1,0)*m(2,2)+m(1,2)*m(2,0))+
			m(0,2)*(m(1,0)*m(2,1)+m(1,1)*m(2,0));
	}

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int		minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int		minRowSize	= rowSize[minRow];

	vector<int>	colSize	   = m.col_nnzs();
	int	minCol	   = min_element(colSize.begin(),colSize.end())-colSize.begin();
	int	minColSize = colSize[minCol];
#ifdef stat
	cerr<<(minRowSize<=minColSize?minRowSize:minColSize)<<endl;
#endif
	if(minRowSize<=minColSize)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

		if(minRowSize==0)
			return 0;

		T ret=0;
		for(int i=0;i<minRowSize/2;i++)
		{
			int	c1	= m.data[minRow].data[0].index;
			T		value1	= m.data[minRow].data[0].value;
			int	c2	= m.data[minRow].data[1].index;
			T		value2	= m.data[minRow].data[1].value;
			m.data[minRow].data.erase(m.data[minRow].data.begin()+1);
			m.data[minRow].data.erase(m.data[minRow].data.begin());
			m._nnz	       -= 2;

			{
				smat<T> mtemp(m);
				eliminate2(mtemp,minRow,c1,c2,value1,value2);
#ifdef plot
				cerr<<"node"<<node<<" eliminate col :"<<c1<<'\t'<<c2<<endl;
				cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
				ret += IDEM(mtemp,node*max+child);
			}
#ifdef plot
			child++;
#endif
		}
		if(!m.data[minRow].data.empty())
		{
			int c= m.data[minRow].data[0].index;
			T value= m.data[minRow].data[0].value;
			eliminate1(m,minRow,c);
#ifdef plot
			cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
			cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
			ret+=value*IDEM(m,node*max+child);
		}
#ifdef plot
		cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
		return ret;
	}
	else
	{
#ifdef plot
		cerr<<"node"<<node<<" min col :"<<minCol<<endl;
#endif
		if(minColSize==0)
			return 0;

		T ret=0;
		vector<int> rows;
		vector<T> values;
		rows.reserve(minColSize);
		values.reserve(minColSize);
		for(unsigned r=0;rows.size()<minColSize;r++)
		{
			T temp=m(r,minCol);
			if(temp!=0)
			{
				rows.push_back(r);
				values.push_back(temp);
			}
		}
		for(unsigned i=0;i<minColSize-1;i+=2)
		{
			int	r1	= rows[i];
			T	value1	= values[i];
			int	r2	= rows[i+1];
			T	value2	= values[i+1];

			{
				smat<T> mtemp(m);
				eliminate2T(mtemp,minCol,r1,r2,value1,value2);
#ifdef plot
				cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
				ret += IDEM(mtemp,node*max+child);
#ifdef plot
				cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
			}
		}
		if(minColSize%2==1)
		{
			int r=rows.back();
			T value=values.back();
			eliminate1(m,r,minCol);
#ifdef plot
			cerr<<"node"<<node<<" eliminate row :"<<r<<endl;
			cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
			ret+=value*IDEM(m,node*max+child);
		}
#ifdef plot
		cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
		return ret;
	}
}

	template<typename T>
T IDEM3(smat<T> &m,int node=1)
{
	if(m.data.size() == 3)
	{
#ifdef plot
		cerr<<"node"<<node<<" return :"<<m(0,0)<<endl;
#endif
#ifdef stat
		cerr<<m.data.size()<<endl;
#endif
		return 
			m(0,0)*(m(1,1)*m(2,2)+m(1,2)*m(2,1))+
			m(0,1)*(m(1,0)*m(2,2)+m(1,2)*m(2,0))+
			m(0,2)*(m(1,0)*m(2,1)+m(1,1)*m(2,0));
	}

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int	minRow= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int	minRowSize	= rowSize[minRow];
	if(minRowSize==0)
		return 0;
	else if(minRowSize==1)
	{
		int c= m.data[minRow].data[0].index;
		T value= m.data[minRow].data[0].value;
		eliminate1(m,minRow,c);
		return value*IDEM3(m,2*node);
	}
	else if(minRowSize==2)
	{
		int	c1     = m.data[minRow].data[0].index;
		int	c2     = m.data[minRow].data[1].index;
		T		value1 = m.data[minRow].data[0].value;
		T		value2 = m.data[minRow].data[1].value;
		eliminate2(m,minRow,c1,c2,value1,value2);
		return IDEM3(m,2*node);
	}
	else
	{
		vector<int>	colSize	  = m.col_nnzs();
		int	minCol	   = min_element(colSize.begin(),colSize.end())-colSize.begin();
		int	minColSize = colSize[minCol];
		if(minColSize==0)
			return 0;
		vector<int> rows;
		vector<T> values;
		rows.reserve(minColSize);
		values.reserve(minColSize);
		for(unsigned r=0;rows.size()<minColSize;r++)
		{
			T temp=m(r,minCol);
			if(temp!=0)
			{
				rows.push_back(r);
				values.push_back(temp);
			}
		}
		if(minColSize==1)
		{
			eliminate1(m,rows[0],minCol);
			return values[0]*IDEM3(m,2*node);
		}
		else
		{
			T ret;
			{
				smat<T> mtemp=m;
				eliminate2(mtemp,minRow,
						mtemp.data[minRow].data[0].index,
						mtemp.data[minRow].data[1].index,
						mtemp.data[minRow].data[0].value,
						mtemp.data[minRow].data[1].value);
				ret=IDEM3(mtemp,2*node);
			}
			T value=m.data[minRow].data[2].value;
			eliminate1(m,minRow,m.data[minRow].data[2].index);
			ret+=value*IDEM3(m,2*node+1);
			return ret;
		}
	}
}

	template<typename T>
T H3(smat<T> &m,int node=1)
{
	if(m.data.size() <= 6)
		return RNW(m.full());

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int	minRow= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int	minRowSize	= rowSize[minRow];
	if(minRowSize==0)
		return 0;
	else if(minRowSize==1)
	{
		int c= m.data[minRow].data[0].index;
		T value= m.data[minRow].data[0].value;
		eliminate1(m,minRow,c);
		return value*H3(m,2*node);
	}
	else if(minRowSize==2)
	{
		int	c1     = m.data[minRow].data[0].index;
		int	c2     = m.data[minRow].data[1].index;
		T	value1 = m.data[minRow].data[0].value;
		T	value2 = m.data[minRow].data[1].value;
		eliminate2(m,minRow,c1,c2,value1,value2);
		return H3(m,2*node);
	}
	else
	{
		vector<int>	colSize	  = m.col_nnzs();
		int	minCol	   = min_element(colSize.begin(),colSize.end())-colSize.begin();
		int	minColSize = colSize[minCol];
		if(minColSize==0)
			return 0;
		vector<int> rows;
		vector<T> values;
		rows.reserve(minColSize);
		values.reserve(minColSize);
		for(unsigned r=0;rows.size()<minColSize;r++)
		{
			T temp=m(r,minCol);
			if(temp!=0)
			{
				rows.push_back(r);
				values.push_back(temp);
			}
		}
		if(minColSize==1)
		{
			eliminate1(m,rows[0],minCol);
			return values[0]*H3(m,2*node);
		}
		//else if(minColSize==2)
		//{
		//	eliminate2T(m,minCol,rows[0],rows[1],values[0],values[1]);
		//	return IDEM3(m,2*node);
		//}
		else
		{
			T ret;
			{
				smat<T> mtemp=m;
				eliminate2(mtemp,minRow,
						mtemp.data[minRow].data[0].index,
						mtemp.data[minRow].data[1].index,
						mtemp.data[minRow].data[0].value,
						mtemp.data[minRow].data[1].value);
				ret=H3(mtemp,2*node);
			}
			T value=m.data[minRow].data[2].value;
			eliminate1(m,minRow,m.data[minRow].data[2].index);
			ret+=value*H3(m,2*node+1);
			return ret;
		}
	}
}

	template<typename T>
T IDEM0(smat<T> &m,int node=0)
{
	int	max   = 3;
	int	child =	1;
#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif

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
#ifdef stat
		cerr<<m.data.size()<<endl;
#endif
		return m(0,0);
	}

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int		minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int		minRowSize	= rowSize[minRow];

#ifdef stat
	cerr<<minRowSize<<endl;
#endif
#ifdef plot
	cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

	if(minRowSize==0)
		return 0;

	T ret=0;
	for(int i=0;i<minRowSize/2;i++)
	{
		int	c1	= m.data[minRow].data[0].index;
		T		value1	= m.data[minRow].data[0].value;
		int	c2	= m.data[minRow].data[1].index;
		T		value2	= m.data[minRow].data[1].value;
		m.data[minRow].data.erase(m.data[minRow].data.begin()+1);
		m.data[minRow].data.erase(m.data[minRow].data.begin());
		m._nnz	       -= 2;

		{
			smat<T> mtemp(m);
			eliminate2(mtemp,minRow,c1,c2,value1,value2);
#ifdef plot
			cerr<<"node"<<node<<" eliminate col :"<<c1<<'\t'<<c2<<endl;
			cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
			ret += IDEM0(mtemp,node*max+child);
#ifdef plot
			child++;
#endif
		}
	}
	if(!m.data[minRow].data.empty())
	{
		int c= m.data[minRow].data[0].index;
		T value= m.data[minRow].data[0].value;
		eliminate1(m,minRow,c);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
		cout<<"\""<<node*max+child<<"\"->\""<<node<<"\";\n";
#endif
		ret+=value*IDEM0(m,node*max+child);
	}
#ifdef plot
	cerr<<"node"<<node<<" return :"<<ret<<endl;
#endif
	return ret;
}

	template<typename T>
void eliminate2T(smat<T> &m,int c,int r1,int r2,T value1,T value2)
{
	m.erase_col(c);

	int i1=0,i2=0;
	while(i1!=-1||i2!=-1)
	{
		if(i1==-1)
		{
#ifdef plot
			cerr<<"node"<<node<<" situation 4"<<" col "<<m.data[r1].data[i1].index<<"\t"<<m.data[r2].data[i2].index<<endl;
#endif
			m.data[r1].data.push_back(element<T>(m.data[r2].data[i2].index,value1*m.data[r2].data[i2].value));
			m._nnz++;
			if(i2==m.data[r2].data.size()-1)
				i2=-1;
			else
				i2++;
		}
		else if(i2==-1)
		{
#ifdef plot
			cerr<<"node"<<node<<" situation 5"<<" col "<<m.data[r1].data[i1].index<<"\t"<<m.data[r2].data[i2].index<<endl;
#endif
			m.data[r1].data[i1].value*=value2;
			if(i1==m.data[r1].data.size()-1)
				i1=-1;
			else
				i1++;
		}
		else if(m.data[r1].data[i1].index<m.data[r2].data[i2].index)
		{
#ifdef plot
			cerr<<"node"<<node<<" situation 1"<<" col "<<m.data[r1].data[i1].index<<"\t"<<m.data[r2].data[i2].index<<endl;
#endif
			m.data[r1].data[i1].value*=value2;
			if(i1==m.data[r1].data.size()-1)
				i1=-1;
			else
				i1++;
		}
		else if(m.data[r1].data[i1].index>m.data[r2].data[i2].index)
		{
#ifdef plot
			cerr<<"node"<<node<<" situation 2"<<" col "<<m.data[r1].data[i1].index<<"\t"<<m.data[r2].data[i2].index<<endl;
			cerr<<"node"<<node<<" i2 = "<<i2<<" "<<m.data[r2].data.size()<<endl;
#endif
			m.data[r1].data.insert(m.data[r1].data.begin()+i1,
					element<T>(m.data[r2].data[i2].index,value1*m.data[r2].data[i2].value));
			m._nnz++;//This is manaul managed
			i1++;//because sth inser before
			if(i2==m.data[r2].data.size()-1)
				i2=-1;
			else
				i2++;
		}
		else
		{
#ifdef plot
			cerr<<"node"<<node<<" situation 3"<<" col "<<m.data[r1].data[i1].index<<"\t"<<m.data[r2].data[i2].index<<" with new value "<<value2*m.data[r1].data[i1].value+value1*m.data[r2].data[i2].value<<endl;
#endif
			m.data[r1].data[i1].value=value2*m.data[r1].data[i1].value+value1*m.data[r2].data[i2].value;
			if(i1==m.data[r1].data.size()-1)
				i1=-1;
			else
				i1++;
			if(i2==m.data[r2].data.size()-1)
				i2=-1;
			else
				i2++;
		}
	}
	m.erase_row(r2);
}
	template<typename T>
void eliminate2(smat<T> &m,int r,int c1,int c2,T value1,T value2)
{
	m.erase_row(r);
	element<T> target1(c1,0),target2(c2,0);
	int		info1,info2;
	int		pos1,pos2;
	for(unsigned r=0;r<m.data.size();r++)
		//eliminate two elements each row
	{
		pos1 = lower_bound_find(m.data[r].data.begin(),m.data[r].data.end(),target1,info1)
			-m.data[r].data.begin();
		if(info1==-1 || info1==2)
			continue;
		pos2 = lower_bound_find(m.data[r].data.begin()+pos1,m.data[r].data.end(),target2,info2)-m.data[r].data.begin();
		//#ifdef plot
		//	      cerr<<"node"<<node<<" row: "<<r<<" pos1,pos2 = "<<pos1<<" , "<<pos2<<endl;
		//	      cerr<<"node"<<node<<" row: "<<r<<" inf1,inf2 = "<<info1<<" , "<<info2<<endl;
		//#endif
		for(unsigned c=pos2;c<m.data[r].data.size();c++)
			m.data[r].data[c].index--;
		if(info1==0)
		{
			if(info2!=0)
				m.data[r].data[pos1].value = value2*m.data[r].data[pos1].value;
			else
			{
				T	tempValue =	value2*m.data[r].data[pos1].value+value1*m.data[r].data[pos2].value;
				if(tempValue==0)
					continue;
				m.data[r].data[pos1].value  =tempValue;
				m.data[r].data.erase(m.data[r].data.begin()+pos2);
				m._nnz--;
			}
			continue;
		}
		if(info1==1)
		{
			if(info2==0)
			{
				//value1!=0
				m.data[r].data.insert(m.data[r].data.begin()+pos1,
						element<T>(c1,value1*m.data[r].data[pos2].value));
				//NOTE erase pos2+1 becuase the insert before
				m.data[r].data.erase(m.data[r].data.begin()+pos2+1);
			}
			continue;
		}
	}
	m._cols--;
}
	template<typename T>
void eliminate1(smat<T> &m,int r,int c)
{
	m.erase_row(r);
	m.erase_col(c);
}
	template<typename T>
T DEM(smat<T> &m,int node=1)
{
#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif

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
#ifdef stat
		cerr<<m.data.size()<<endl;
#endif
		return m(0,0);
	}

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int	minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int	minRowSize	= rowSize[minRow];

#ifdef stat
	cerr<<minRowSize<<endl;
#endif
	if(minRowSize==0)
	{
		return 0;
#ifdef plot
		cerr<<"node"<<node<<" return "<<0<<endl;
#endif
	}
	else if(minRowSize==1)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

		int c= m.data[minRow].data[0].index;
		T value= m.data[minRow].data[0].value;
		eliminate1(m,minRow,c);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
		cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=green];\n";
#endif
		return value*DEM(m,2*node);
	}
	else if(minRowSize==2)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

		int	c1     = m.data[minRow].data[0].index;
		int	c2     = m.data[minRow].data[1].index;
		T		value1 = m.data[minRow].data[0].value;
		T		value2 = m.data[minRow].data[1].value;
		eliminate2(m,minRow,c1,c2,value1,value2);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c1<<','<<c2<<endl;
		cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=blue];\n";
#endif
		return DEM(m,2*node);
	}
	else
	{
		vector<int>	colSize	 = m.col_nnzs();
		int		maxCol	 = max_element(colSize.begin(),colSize.end())-colSize.begin();
		int		r;
		for(r=0;r<m.data.size();r++)
			if (m(r,maxCol)		!= 0)
				break;
#ifdef plot
		cerr<<"node"<<node<<" eliminate row :"<<r<<endl;
#endif

		int		icc;	//index of column where c belongs to
		for(icc = 0;icc<3;icc++)
			if(m.data[r].data[icc].index==maxCol)
				break;
		int c1,c2;
		T value1,value2;
		if(icc==0)
		{
			c1     = m.data[r].data[1].index;
			c2     = m.data[r].data[2].index;
			value1 = m.data[r].data[1].value;
			value2 = m.data[r].data[2].value;
		}
		else if(icc==1)
		{
			c1     = m.data[r].data[0].index;
			c2     = m.data[r].data[2].index;
			value1 = m.data[r].data[0].value;
			value2 = m.data[r].data[2].value;
		}
		else
		{
			c1     = m.data[r].data[0].index;
			c2     = m.data[r].data[1].index;
			value1 = m.data[r].data[0].value;
			value2 = m.data[r].data[1].value;
		}

		T p=0;
		{
			smat<T> mtemp=m;
			eliminate2(mtemp,r,c1,c2,value1,value2);
#ifdef plot
			cerr<<"node"<<node<<" eliminate col :"<<c1<<','<<c2<<endl;
			cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=blue];\n";
#endif
			p=DEM(mtemp,2*node);
		}	

		T value = m.data[r].data[icc].value;
		eliminate1(m,r,maxCol);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<maxCol<<endl;
		cout<<"\""<<2*node+1<<"\"->\""<<node<<"\"[color=green];\n";
#endif
		p	+= value*DEM(m,2*node+1);
		return p;
	}
}
	template<typename T>
T DEM2(smat<T> &m,int node=1)
{
#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif

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
#ifdef stat
		cerr<<m.data.size()<<endl;
#endif
		return m(0,0);
	}

	//find the row with minimal element
	vector<int>	rowSize(m.data.size());
	for(unsigned r=0;r<rowSize.size();r++)
		rowSize[r] = m.data[r].data.size();
	int	minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int	minRowSize	= rowSize[minRow];

#ifdef stat
	cerr<<minRowSize<<endl;
#endif
	if(minRowSize==0)
	{
		return 0;
#ifdef plot
		cerr<<"node"<<node<<" return "<<0<<endl;
#endif
	}
	else if(minRowSize==1)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

		int c= m.data[minRow].data[0].index;
		T value= m.data[minRow].data[0].value;
		eliminate1(m,minRow,c);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c<<endl;
		cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=green];\n";
#endif
		return value*DEM(m,2*node);
	}
	else if(minRowSize==2)
	{
#ifdef plot
		cerr<<"node"<<node<<" min row :"<<minRow<<endl;
#endif

		int	c1     = m.data[minRow].data[0].index;
		int	c2     = m.data[minRow].data[1].index;
		T		value1 = m.data[minRow].data[0].value;
		T		value2 = m.data[minRow].data[1].value;
		eliminate2(m,minRow,c1,c2,value1,value2);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c1<<','<<c2<<endl;
		cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=blue];\n";
#endif
		return DEM(m,2*node);
	}
	else
	{
		vector<int>	colSize	 = m.col_nnzs();
		int	minCol	   = min_element(colSize.begin(),colSize.end())-colSize.begin();
		int	minColSize = colSize[minCol];
		if(minColSize==0)
			return 0;
		if(minColSize==1)
		{
			for(int r=0;r<m.data.size();r++)
			{
				T temp=m(r,minCol);
				if (temp != 0)
				{
					eliminate1(m,r,minCol);
			return temp*IDEM3(m,2*node);
				}
			}
		}
		int		maxCol	 = max_element(colSize.begin(),colSize.end())-colSize.begin();
		int		r;
		for(r=0;r<m.data.size();r++)
			if (m(r,maxCol)		!= 0)
				break;
#ifdef plot
		cerr<<"node"<<node<<" eliminate row :"<<r<<endl;
#endif

		int		icc;	//index of column where c belongs to
		for(icc = 0;icc<3;icc++)
			if(m.data[r].data[icc].index==maxCol)
				break;
		int c1,c2;
		T value1,value2;
		if(icc==0)
		{
			c1     = m.data[r].data[1].index;
			c2     = m.data[r].data[2].index;
			value1 = m.data[r].data[1].value;
			value2 = m.data[r].data[2].value;
		}
		else if(icc==1)
		{
			c1     = m.data[r].data[0].index;
			c2     = m.data[r].data[2].index;
			value1 = m.data[r].data[0].value;
			value2 = m.data[r].data[2].value;
		}
		else
		{
			c1     = m.data[r].data[0].index;
			c2     = m.data[r].data[1].index;
			value1 = m.data[r].data[0].value;
			value2 = m.data[r].data[1].value;
		}

		T p=0;
		{
			smat<T> mtemp=m;
			eliminate2(mtemp,r,c1,c2,value1,value2);
#ifdef plot
			cerr<<"node"<<node<<" eliminate col :"<<c1<<','<<c2<<endl;
			cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=blue];\n";
#endif
			p=DEM(mtemp,2*node);
		}	

		T value = m.data[r].data[icc].value;
		eliminate1(m,r,maxCol);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<maxCol<<endl;
		cout<<"\""<<2*node+1<<"\"->\""<<node<<"\"[color=green];\n";
#endif
		p	+= value*DEM(m,2*node+1);
		return p;
	}
}
	template<typename T>
void peelDEM(smat<T> &m,bool& end,T& ret)
{
	end=false;
	ret=1;
	while(true)
	{
		if(m.data.size()==1)
		{
			end=true;
			ret*=m(0,0);
			return;
		}
		vector<int>	rowSize(m.data.size());
		for(unsigned r=0;r<rowSize.size();r++)
			rowSize[r] = m.data[r].data.size();
		int	minRow	= min_element(rowSize.begin(),rowSize.end())-rowSize.begin();
		int	minRowSize	= rowSize[minRow];
		if(minRowSize==0)
		{
			end=true;
			ret=0;
			return;
		}
		else if(minRowSize==1)
		{

			int c= m.data[minRow].data[0].index;
			ret*=m.data[minRow].data[0].value;
			eliminate1(m,minRow,c);
		}
		else if(minRowSize==2)
		{

			int	c1     = m.data[minRow].data[0].index;
			int	c2     = m.data[minRow].data[1].index;
			T		value1 = m.data[minRow].data[0].value;
			T		value2 = m.data[minRow].data[1].value;
			eliminate2(m,minRow,c1,c2,value1,value2);
		}
		else//minRowSize==3
			return;
	}
}
	template<typename T>
T DEMiter(smat<T> &m,int node=1)
{
#ifdef stat
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif

#ifdef plot
	cout<<"\""<<node<<"\"[label=\"node: "<<node<<"\\n";
	m.print();
	cout<<"\"];\n";
#endif
	vector<int>	colSize	 = m.col_nnzs();
	int		maxCol	 = max_element(colSize.begin(),colSize.end())-colSize.begin();
	int		r;
	for(r=0;r<m.data.size();r++)
		if (m(r,maxCol)	!= 0)
			break;
#ifdef plot
	cerr<<"node"<<node<<" eliminate row :"<<r<<endl;
#endif

	int		icc;	//index of column where c belongs to
	for(icc = 0;icc<3;icc++)
		if(m.data[r].data[icc].index==maxCol)
			break;
	int c1,c2;
	T value1,value2;
	if(icc==0)
	{
		c1     = m.data[r].data[1].index;
		c2     = m.data[r].data[2].index;
		value1 = m.data[r].data[1].value;
		value2 = m.data[r].data[2].value;
	}
	else if(icc==1)
	{
		c1     = m.data[r].data[0].index;
		c2     = m.data[r].data[2].index;
		value1 = m.data[r].data[0].value;
		value2 = m.data[r].data[2].value;
	}
	else
	{
		c1     = m.data[r].data[0].index;
		c2     = m.data[r].data[1].index;
		value1 = m.data[r].data[0].value;
		value2 = m.data[r].data[1].value;
	}

	T p;
	{
		smat<T> mtemp=m;
		eliminate2(mtemp,r,c1,c2,value1,value2);
#ifdef plot
		cerr<<"node"<<node<<" eliminate col :"<<c1<<','<<c2<<endl;
		cout<<"\""<<2*node<<"\"->\""<<node<<"\"[color=blue];\n";
#endif
		bool end;T ret;
		peelDEM(mtemp,end,ret);
		if(end)
			p=ret;
		else
			p=ret*DEMiter(mtemp);
	}

	T value = m.data[r].data[icc].value;
	eliminate1(m,r,maxCol);
#ifdef plot
	cerr<<"node"<<node<<" eliminate col :"<<maxCol<<endl;
	cout<<"\""<<2*node+1<<"\"->\""<<node<<"\"[color=green];\n";
#endif
	bool end;T ret;
	peelDEM(m,end,ret);
	if(end)
		p+=value*ret;
	else
		p+=value*ret*DEMiter(m);
	return p;
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
	//cout<<100*(double)_nnz/(_cols*_cols)<<" %\\n";
	cout<<_nnz<<"\\n";
	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<_cols;j++)
			cout<<this->operator()(i,j)<<' ';
		cout<<"\\n";
	}
}
	template <typename T>
vector<int> aPerfectMatch(const smat<T> &m,int& trytimes)
{
	vector<int>	ret;
	trytimes = 1;
tryagain:
	//if(trytimes > 10000)
	//  return ret;
	for(int r=0;r<m.data.size();r++)
	{
		vector<int>	candidates;
		for(int i=0;i<m.data[r].data.size();i++)
		{
			int candidate = m.data[r].data[i].index;
			if(ret.empty()||find(ret.begin(),ret.end(),candidate)==ret.end())
				candidates.push_back(candidate);
		}
#ifdef debug
		cout<<"At row : "<<r<<endl;
		cout<<"ret = ";
		printVector(ret);
		cout<<"candidates = ";
		printVector(candidates);
#endif
		if(candidates.empty())
		{
			ret.clear();
			trytimes++;
			goto tryagain;
		}
		else
			ret.push_back(candidates[randint(candidates.size())]);
	}
	return ret;
}
#endif
/* iSparseMatrix.h ends here */
