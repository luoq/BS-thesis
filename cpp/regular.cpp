// regular.cpp ---
//
// Filename: regular.cpp
// Description:
// Author: Luo Qiang
// Created: 03/15/2010 10:04:55
// Version:
// Last-Updated: 06/14/2010 19:18:59
//           By: Luo Qiang
//     Update #: 320
// Keywords:

// Commentary:

// Change log:

// Code:
#include "regular.h"

//get the group of a point
int	group(int point,int d);
void	printMatrix(const smat<int> &matrix);

smat<int> regular(int n,int d){
  smat<int> matrix(n,n,d);
#ifdef prob
  double K=1.0;
#endif
  if(n*d%2!=0){
    return matrix;
  }
#ifdef debug
  cout<<matrix<<endl;
#endif
 tryagain:
  vector<int> points(n*d);
  vector<int> suitablePoints,suitablePointsIndex;
  for(unsigned i=0;i!=points.size();++i){
    points[i]=i;
  }
  while(!points.empty()){
    int	index1 = randint(points.size());
    int	group1 = group(points[index1],d);
#ifdef debug
    //cout<<"matrix: \n";
    //printMatrix(matrix);
    cout<<"points :\n";
    cout<<points;
    //cout<<"1st point: "<<points[index1]<<endl;
#endif
    suitablePoints.clear();
    suitablePointsIndex.clear();
    for(unsigned i=0;i!=points.size();++i){
      int group2 = group(points[i],d);
      if(group2!=group1 && matrix(group1,group2) == 0){
	suitablePoints.push_back(points[i]);
	suitablePointsIndex.push_back(i);
      }
    }
    if(suitablePoints.empty()){
#ifdef debug
      cout<<"try again"<<endl;
#endif
      goto tryagain;
    }
#ifdef prob
    K*=suitablePoints.size();
#endif
    int index2	 = randint(suitablePoints.size());
    int group2	 = group(suitablePoints[index2],d);
    matrix.set(group1,group2,1);
    matrix.set(group2,group1,1);
#ifdef debug
    cout<<"points to be deleted:\n";
    cout<<points[index1]<<' '<<suitablePoints[index2]<<endl;
    cout<<"add: ("<<group1<<','<<group2<<")\n";
    if(matrix(group1,group2) !=	1)
      {
	cout<<matrix;
	exit(1);
      }
    cout<<"add: ("<<group2<<','<<group1<<")\n";
    if(matrix(group2,group1) !=	1)
      {
	cout<<matrix;
	exit(1);
      }
#endif
    points.erase(points.begin()+index1);
    //NOTE,when index1<suitablePointsIndex[index2],the index of second point
    //changed as the erase of first point
    if(index1<suitablePointsIndex[index2])
      points.erase(points.begin()+suitablePointsIndex[index2]-1);
    else
      points.erase(points.begin()+suitablePointsIndex[index2]);
  }
#ifdef prob
  cout<<"1/p="<<K<<endl;
#endif
  return matrix;
}
int group(int point,int d){
  return int(point/d);
}
//naive method changed with no prob guarantee but faster
smat<int> gen_with_nnzs(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes)
{
  vector<int>	lefted_in_col;
  vector<int>	candidates;
  vector<int>	choosed_cols;
  int		r,c,j;
#ifdef prob
  double P;
#endif
  smat<int>	m(row_nnzs.size(),col_nnzs.size());
  candidates.reserve(col_nnzs.size());
  trytimes	= 0;
 tryagain:
#ifdef prob
  P=1;
#endif
  trytimes++;
  lefted_in_col	= col_nnzs;
  candidates.clear();
  m.clear();
#ifdef debug
  cout<<"try time "<<trytimes<<endl;
#endif
  for (r = 0; r<row_nnzs.size(); ++r)
    {
#ifdef debug
      cout<<"handle row "<<r<<endl;
#endif
      for(c=0;c<col_nnzs.size();c++)
	if(lefted_in_col[c]!=0)
	  candidates.push_back(c);
      if(candidates.size()<row_nnzs[r])
	{
#ifdef debug
	  cout<<"try again\n";
#endif
	  goto tryagain;
	}
#ifdef prob
      double C=1.0;//choose row_nnzs[r] from candidates.size()
      for(int i=0;i<row_nnzs[r];i++)
	C*=double(candidates.size()-i)/double(row_nnzs[r]-i);
      P/=C;
#endif
      choosed_cols = chooseKfromN(candidates.size(),row_nnzs[r]);
      for(j=0;j<choosed_cols.size();j++)
	{
	  choosed_cols[j]=candidates[choosed_cols[j]];
	  m.set(r,choosed_cols[j],1);
	  lefted_in_col[choosed_cols[j]]--;
	}
#ifdef debug
      cout<<choosed_cols<<endl;
#endif
      candidates.clear();
    }
#ifdef prob
  cout<<"prob:"<<P<<endl;
#endif
  return m;
}
//naive method
smat<int> gen_with_nnzs0(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes)
{
  vector<int>	lefted_in_col;
  vector<int>	choosed_cols;
  int		r,c,j;
#ifdef prob
  double K;
#endif
  smat<int>	m(row_nnzs.size(),col_nnzs.size());
  trytimes	= 0;
 tryagain:
  trytimes++;
  lefted_in_col	= col_nnzs;
  m.clear();
#ifdef debug
  cout<<"try time "<<trytimes<<endl;
#endif
  for (r = 0; r<row_nnzs.size(); ++r)
    {
      choosed_cols = chooseKfromN(col_nnzs.size(),row_nnzs[r]);
      for(j=0;j<choosed_cols.size();j++)
	{
	  if(lefted_in_col[choosed_cols[j]]>0)
	    {
	      m.set(r,choosed_cols[j],1);
	      lefted_in_col[choosed_cols[j]]--;
	    }
	  else
	    goto tryagain;
	}
#ifdef debug
      cout<<choosed_cols<<endl;
#endif
    }
  return m;
}
//method similar to Steger1999
smat<int> gen_with_nnzs2(const vector<int> row_nnzs,const  vector<int> col_nnzs,int &trytimes)
{
  vector<int>	lefted_in_col,lefted_in_row;
  smat<int>	m(row_nnzs.size(),col_nnzs.size());
  vector<int>	candidates;
  candidates.reserve(col_nnzs.size());
  trytimes = 0;
#ifdef prob
  double P;
#endif
 tryagain:
#ifdef prob
  P=1.0;
#endif
  trytimes++;
  m.clear();
  lefted_in_col	= col_nnzs;
  lefted_in_row	= row_nnzs;
  while(1)
    {
      int	i = choose1_with_weight(lefted_in_row);
      if(i==-1)
	break;

      //check dead end
      candidates.clear();
      for(int c=0;c<lefted_in_col.size();c++)
	if(lefted_in_col[c]!=0&&m(i,c) == 0)
	  candidates.push_back(c);
      if(candidates.empty())
	goto tryagain;

      int	j = choose1_with_weight(lefted_in_col);
#ifdef prob
	  P	 *= lefted_in_row[i]*lefted_in_col[j];
#endif
      if(m(i,j) == 0)
	{
	  m.set(i,j,1);
	  lefted_in_row[i]--;
	  lefted_in_col[j]--;
	}
    }
#ifdef prob
  cout<<"prob: "<<P<<endl;
#endif
  return m;
}
//method similar to Steger1999 with probability guarantee
smat<int> gen_with_nnzs3(const vector<int> row_nnzs,const  vector<int> col_nnzs,int &trytimes)
{
  vector<int>	lefted_in_col,lefted_in_row;
  smat<int>	m(row_nnzs.size(),col_nnzs.size());
  vector<int>	candidates;
  candidates.reserve(col_nnzs.size());
  trytimes = 0;
 tryagain:
  trytimes++;
  m.clear();
  lefted_in_col	= col_nnzs;
  lefted_in_row	= row_nnzs;
  while(1)
    {
      int	i = choose1_with_weight(lefted_in_row);
      if(i==-1)
	break;

      int	j = choose1_with_weight(lefted_in_col);
      if(m(i,j) == 0)
	{
	  m.set(i,j,1);
	  lefted_in_row[i]--;
	  lefted_in_col[j]--;
	}
	  else
		  goto tryagain;
    }
  return m;
}

//method similar to Steger1999
smat<int> regular2(int n,int d,int &trytimes)
{
  vector<int>	lefted_in_col(n,d),lefted_in_row(n,d);
  smat<int>	m(n,n,d);
  vector<int>	candidates;
  candidates.reserve(n);
  trytimes = 0;
#ifdef prob
  double P;
#endif
 tryagain:
#ifdef prob
  P=1.0;
#endif
  trytimes++;
  m.clear();
  lefted_in_col.assign(n,d);
  lefted_in_row.assign(n,d);
  while(1)
    {
      int	i = choose1_with_weight(lefted_in_row);
      if(i==-1)
	break;

      //check dead end
      candidates.clear();
      for(int c=0;c<lefted_in_col.size();c++)
	if(lefted_in_col[c]!=0&&i!=c&&m(i,c) == 0)
	  candidates.push_back(c);
      if(candidates.empty())
	goto tryagain;

      int	j = choose1_with_weight(lefted_in_col);
      if(i!=j&&m(i,j) == 0)
	{
	  m.set(i,j,1);
	  m.set(j,i,1);
#ifdef prob
	  P	 *= lefted_in_row[i]*lefted_in_col[j];
#endif
	  lefted_in_row[i]--;
	  lefted_in_col[i]--;
	  lefted_in_col[j]--;
	  lefted_in_row[j]--;
	}
    }
#ifdef prob
  cout<<"prob: "<<P<<endl;
#endif
  return m;
}
smat<int> mix_regular(int n,int k,int d,int& trytimes)
{
  vector<int>	row_nnzs(n-k,d-1),col_nnzs(n-k,d-1);
  for(int i=0;i<k;i++)
    {
      row_nnzs.push_back(d);
      col_nnzs.push_back(d);
    }
  return gen_with_nnzs3(row_nnzs,col_nnzs,trytimes);
}
// regular.cpp ends here
