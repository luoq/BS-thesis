#ifndef PERMANENR_H
#define PERMANENR_H
#include "iSparseMatrix.h"
#include "iFullMatrix.h"
#include <cmath>
//eliminate minimal row
template<typename T> T	IDEM0(smat<T> &m,int node=0);
//eliminate minimal row and column
template<typename T> T	IDEM(smat<T> &m,int node=0);
//hybrd IDEM and RNW
template<typename T> T	H(smat<T> &m,int node=0);
const int changePoint=8;
int _next[127],_addOrRemove[127],_card[127];
void generate_travel_order()
{
	subsetGenerator g(changePoint-1);
	int i=0;
	while(!g.end())
	{
		g.next(_next[i],_addOrRemove[i],_card[i]);
		i++;
	}
}


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
template<typename T> T RNW(fmat<T> m);
template<typename T> T RNW_presave(fmat<T> m);


template<typename T> vector<int> selectElements(const smat<T>&,int& trytimes);
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


	template<typename T>
T H(smat<T> &m,int node=1)
{
	int	max   = 4;
	int	child =	1;

	if(m.data.size() <= changePoint)
	{
#ifdef plot
		cout<<"\""<<node<<"\"[label=\"";
		//m.print();
		cout<<"end";
		cout<<"\",fillcolor=red];\n";
#endif
		return RNW_presave(m.full());
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
	int		maxRow	= max_element(rowSize.begin(),rowSize.end())-rowSize.begin();
	int		maxRowSize	= rowSize[maxRow];
	int		maxCol	   = max_element(colSize.begin(),colSize.end())-colSize.begin();
	int		maxColSize = colSize[maxCol];
	cerr<<m.data.size()<<'\t'<<minRowSize<<'\t'<<maxRowSize<<'\t'<<minColSize<<'\t'<<maxColSize<<endl;
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
		for(int i=0;i<minRowSize-1;i+=2)
		{
			int	c1	= m.data[minRow].data[i].index;
			T		value1	= m.data[minRow].data[i].value;
			int	c2	= m.data[minRow].data[i+1].index;
			T		value2	= m.data[minRow].data[i+1].value;

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
		if(minRowSize%2==1)
		{
			int c= m.data[minRow].data[minRowSize-1].index;
			T value= m.data[minRow].data[minRowSize-1].value;
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
#ifndef nonnz
			m._nnz	       -= 2;
#endif
#ifdef colnnzs
			m._col_nnzs[c1]--;
			m._col_nnzs[c2]--;
#endif

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
#ifndef nonnz
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif
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
#ifndef nonnz
		m._nnz	       -= 2;
#endif
#ifdef colnnzs
		m._col_nnzs[c1]--;
		m._col_nnzs[c2]--;
#endif

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
			m.data[r1].data.push_back(element<T>(m.data[r2].data[i2].index,value1*m.data[r2].data[i2].value));
#ifndef nonnz
			m._nnz++;
#endif
#ifdef colnnzs
			m._col_nnzs[m.data[r2].data[i2].index]++;
#endif
			if(i2==m.data[r2].data.size()-1)
				i2=-1;
			else
				i2++;
		}
		else if(i2==-1)
		{
			m.data[r1].data[i1].value*=value2;
			if(i1==m.data[r1].data.size()-1)
				i1=-1;
			else
				i1++;
		}
		else if(m.data[r1].data[i1].index<m.data[r2].data[i2].index)
		{
			m.data[r1].data[i1].value*=value2;
			if(i1==m.data[r1].data.size()-1)
				i1=-1;
			else
				i1++;
		}
		else if(m.data[r1].data[i1].index>m.data[r2].data[i2].index)
		{
			m.data[r1].data.insert(m.data[r1].data.begin()+i1,
					element<T>(m.data[r2].data[i2].index,value1*m.data[r2].data[i2].value));
#ifndef nonnz
			m._nnz++;//This is manaul managed
#endif
#ifdef colnnzs
			m._col_nnzs[m.data[r2].data[i2].index]++;
#endif
			i1++;//because sth inser before
			if(i2==m.data[r2].data.size()-1)
				i2=-1;
			else
				i2++;
		}
		else
		{
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
#ifndef nonnz
				m._nnz--;
#endif
			}
			//continue;
		}
		if(info1==1)
		{
			if(info2==0)
			{
				//value1!=0
				m.data[r].data.insert(m.data[r].data.begin()+pos1,
						element<T>(c1,value1*m.data[r].data[pos2].value));
#ifdef colnnzs
				m._col_nnzs[c1]++;
#endif
				//NOTE erase pos2+1 becuase the insert before
				m.data[r].data.erase(m.data[r].data.begin()+pos2+1);
			}
			//continue;
		}
	}
#ifdef colnnzs
	m._col_nnzs.erase(m._col_nnzs.begin()+c2);
#endif
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
#ifndef nonnz
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif
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
#ifndef nonnz
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif
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
#ifndef nonnz
	cerr<<m.data.size()<<'\t'<<m._nnz<<'\t';
#endif
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

template<typename T>
T RNW(fmat<T> m)
{
  int	n = m.rows();
  T	P = 0,sigma=1;
  T *	S = new T[n];
  for(int r=0;r<n;r++)
    S[r]=0;
  
  for(int r=0;r<n;r++)
    {
      for(int c=0;c<n;c++)
	S[r] +=	m(r,c);
      S[r]   *=	-1;
      S[r]   +=	2*m(r,n-1);
    }

  int	index,addOrRemove,card;
  subsetGenerator	g(n-1);
  for(int r=0;r<n;r++)
    sigma *= S[r];
  P	  += sigma;
#ifdef debug_rnw
  for(int r=0;r<n;r++)
    cout<<S[r]<<' ';
  cout<<endl;
  cout<<"sigma = "<<sigma<<endl;
  cout<<" P = "<<P<<endl<<endl;
#endif
  sigma	   = 1;
  
  while(!g.end())
    {
      g.next(index,addOrRemove,card);
      for (int r = 0; r<n;r++)
	S[r]  += 2*addOrRemove*m(r,index);
      for(int r=0;r<n;r++)
	sigma *= S[r];
      if(card%2==1)
	sigma=-sigma;
      P	  += sigma;
#ifdef debug_rnw
      for(int r=0;r<n;r++)
	cout<<S[r]<<' ';
      cout<<endl;
      cout<<"sigma = "<<sigma<<endl;
      cout<<" P = "<<P<<endl<<endl;
#endif
      sigma=1;
    }
  P /= pow(2,n-1);
  delete[] S;
  if(n%2==1)
    return P;
  else
    return -P;
}
template<typename T>
T RNW_presave(fmat<T> m)
{
  int	n=changePoint;
  T	P = 0,sigma=1;
  T *	S = new T[n];
  for(int r=0;r<n;r++)
    S[r]=0;
  
  for(int r=0;r<n;r++)
    {
      for(int c=0;c<n;c++)
	S[r] +=	m(r,c);
      S[r]   *=	-1;
      S[r]   +=	2*m(r,n-1);
    }

  int	index,addOrRemove,card;
  subsetGenerator	g(n-1);
  for(int r=0;r<n;r++)
    sigma *= S[r];
  P	  += sigma;
#ifdef debug_rnw
  for(int r=0;r<n;r++)
    cout<<S[r]<<' ';
  cout<<endl;
  cout<<"sigma = "<<sigma<<endl;
  cout<<" P = "<<P<<endl<<endl;
#endif
  
  for(int i=0;i<pow(2,changePoint-1)-1;i++)
    {
		sigma	   = 1;
	  index=_next[i];addOrRemove=_addOrRemove[i];card=_card[i];
      for (int r = 0; r<n;r++)
	S[r]  += 2*addOrRemove*m(r,index);
      for(int r=0;r<n;r++)
	sigma *= S[r];
      if(card%2==1)
	sigma=-sigma;
      P	  += sigma;
#ifdef debug_rnw
      for(int r=0;r<n;r++)
	cout<<S[r]<<' ';
      cout<<endl;
      cout<<"sigma = "<<sigma<<endl;
      cout<<" P = "<<P<<endl<<endl;
#endif
    }
  P /= pow(2,n-1);
  delete[] S;
  if(n%2==1)
    return P;
  else
    return -P;
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
vector<int> selectElements(const smat<T> &m,int k,int& trytimes)
{
	vector<int>	ret;
	trytimes = 1;
	vector<int> rows=chooseKfromN(m.data.size(),k);
tryagain:
	//if(trytimes > 10000)
	//  return ret;
	for(int r=0;r<k;r++)
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
