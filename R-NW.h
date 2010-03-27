/* R-NW.h --- 
 * 
 * Filename: R-NW.h
 * Description:R-NW method to compute permanet 
 * Author: Luo Qiang
 * Created: 03/23/2010 15:38:00
 * Version: 
 * Last-Updated: 03/27/2010 19:46:35
 *           By: Luo Qiang
 *     Update #: 36
 * Keywords: 

/* Commentary: 

/* Change log:

/* Code: */
#ifndef R_NW_H
#define R_NW_H

#include "iFullMatrix.h"
#include <cmath>


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
#endif
/* R-NW.h ends here */
