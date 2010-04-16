/* misc.h --- 
 * 
 * Filename: misc.h
 * Description:miscellaneous functions
 * Author: Luo Qiang
 * Created: 03/23/2010 10:30:53
 * Version: 
 * Last-Updated: 03/27/2010 17:14:55
 *           By: Luo Qiang
 *     Update #: 37
 * Keywords: */

#ifndef MSIC_H
#define MSIC_H
#include	<stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;
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
#endif
/* misc.h ends here */
