#include <sys/time.h>
#include <stdlib.h> 
void wtime_(double* time)
{
	double tim[2];
	struct timeval time1; 
	int ierr; 
	double elap; 
	ierr = gettimeofday(&time1, NULL) ; 
	tim[0] = time1.tv_sec;
	tim[1] = time1.tv_usec; 
	*time=tim[0]+tim[1]*1e-6;
}
