/* misc.h --- 
 * 
 * Filename: misc.h
 * Description:miscellaneous functions
 * Author: Luo Qiang
 * Created: 03/23/2010 10:30:53
 * Version: 
 * Last-Updated: 03/23/2010 12:22:12
 *           By: Luo Qiang
 *     Update #: 20
 * Keywords: 

/* Commentary: 

/* Change log:

/* Code: */
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
}
#endif
/* misc.h ends here */
