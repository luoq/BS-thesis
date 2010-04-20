/* misc.h --- 
 * 
 * Filename: misc.h
 * Description:miscellaneous functions
 * Author: Luo Qiang
 * Created: 03/23/2010 10:30:53
 * Version: 
 * Last-Updated: 04/20/2010 16:49:44
 *           By: Luo Qiang
 *     Update #: 41
 * Keywords: */

#ifndef MSIC_H
#define MSIC_H
#include	<stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

//info = -1,empty
//info = 0,find
//info = 1,not find,return first larger
//info = 2,not find,at end
template <class	ForwardIterator, class T>
ForwardIterator lower_bound_find(ForwardIterator first, ForwardIterator last,
				 const T& value,int & info);
//generate a random integer from 0 to n
int randint(int n);
template<typename T> void printVector(const vector<T> &vect);

template<typename T> 
ostream& operator<<(ostream& out,const vector<T> &vect)
{
  for(typename vector<T>::const_iterator iter=vect.begin();iter != vect.end();++iter)
    out<<*iter<<' ';
  out<<endl;
  return out;
}

//return next subset in gray code order
class subsetGenerator
{
public:
  subsetGenerator(int n);
  void	next(int& nextIndex,int & addOrRemove,int& k);
  bool end(){return _end;}
  ~subsetGenerator();
private:
  int n;
  int _k;
  bool _end;
  bool* set;
};
vector<int> chooseKfromN(int n,int k);
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
#endif
/* misc.h ends here */
