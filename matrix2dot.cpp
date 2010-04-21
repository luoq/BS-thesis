#include "iSparseMatrix.h"
int main(int argc,char** argv)
{
	if(argc!=3)
	{
		cout<<"usage; "<<argv[0]<<" path/to/load path/to/save\n";
		return 1;
	}
	char* path=argv[1];
	char* savepath=argv[2];
	ifstream in(path);
	ofstream out(savepath);
	if(!in)
	{
		cout<<"cannot open file\n";
		return 2;
	}

	//get head and parse
	string	head;
	int		minindex   = 0;
	getline(in,head);
	if(head.find("Octave ") != string::npos)
		minindex=1;

	//get matrix size and nnz
	int nnz,rows,cols;
	in.ignore(1024,'\n');
	in.ignore(1024,'\n');
	in.ignore(1024,':');
	in>>nnz;
	in.ignore(1024,':');
	in>>rows;
	in.ignore(1024,':');
	in>>cols;


	out<<"graph \""<<path<<"\" {\n";
	int	r,c;
	int value;
	while(in>>r>>c>>value)
	{
		r	-= minindex;
		c	-= minindex;
		if(r<=c)
			out<<'\t'<<r<<"\t--\t"<<c<<";\n";
	}
	out<<"}\n";
	in.close();
	out.close();
	return 0;
}
