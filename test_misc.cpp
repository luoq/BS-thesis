#include "misc.h"
int main(int argc,char** argv)
{
	if(argc!=3)
	{
		cout<<"usage: "<<argv[0]<<" n k\n";
		return 1;
	}
	int n=atoi(argv[1]);
	int k=atoi(argv[2]);
	srand(time(NULL));
	for(int i=0;i<720;i++)
		cout<<chooseKfromN(n,k);
	return 0;
}
