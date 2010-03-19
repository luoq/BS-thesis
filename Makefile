CC=g++
#CC=icpc
#CXXFLAGS=-O3
CXXFLAGS=-march=i686 -mtune=native -O3 -pipe
#CXXFLAGS=-O3
test:test.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ test.cpp regular.cpp
main:main.o regular.o
	$(CC) $^ -o $@
main.o:main.cpp regular.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h iSparseMatrix.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o main data test
