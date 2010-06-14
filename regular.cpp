// regular.cpp ---
//
// Filename: regular.cpp
// Description:
// Author: Luo Qiang
// Created: 03/15/2010 10:04:55
// Version:
// Last-Updated: 06/14/2010 08:12:56
//           By: Luo Qiang
//     Update #: 223
// Keywords:

// Commentary:

// Change log:

// Code:
#include "regular.h"

//get the group of a point
int	group(int point,int d);
void	printMatrix(const smat<int> &matrix);

smat<int> regular(int n,int d,int &trytimes){
  smat<int> matrix(n,n,d);
#ifdef prob
  double K;
#endif
  trytimes=1;
  if(n*d%2!=0){
    return matrix;
  }
#ifdef debug
  cout<<matrix<<endl;
#endif
  vector<int> points(n*d);
  vector<int> suitablePoints,suitablePointsIndex;
 tryagain:
#ifdef prob
  K=1;
#endif
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
      matrix.clear();
      points.resize(n*d);
      trytimes++;
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
smat<int> gen_with_nnzs(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes)
{
  vector<int>	lefted_in_col;
  vector<int>	candidates;
  vector<int>	choosed_cols;
  int		r,c,j;
#ifdef prob
  double K;
#endif
  smat<int>	m(row_nnzs.size(),col_nnzs.size());
  candidates.reserve(col_nnzs.size());
  trytimes	= 0;
 tryagain:
#ifdef prob
  K=1;
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
      K*=C;
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
  cout<<"1/p="<<K<<endl;
#endif
  return m;
}
smat<int> gen_with_nnzs2(const vector<int> row_nnzs,const  vector<int> col_nnzs,int& trytimes)
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
smat<int> regular2(int n,int d,int &trytimes)
{
  vector<int>	row_nnzs(n,d),col_nnzs(n,d);
  return gen_with_nnzs(row_nnzs,col_nnzs,trytimes);
}
smat<int> mix_regular(int n,int k,int d,int& trytimes)
{
  vector<int>	row_nnzs(n-k,d-1),col_nnzs(n-k,d-1);
  for(int i=0;i<k;i++)
    {
      row_nnzs.push_back(d);
      col_nnzs.push_back(d);
    }
  return gen_with_nnzs(row_nnzs,col_nnzs,trytimes);
}
// regular.cpp ends here
