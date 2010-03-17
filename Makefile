CC=g++
CXXFLAGS=-g
#CXXFLAGS=-march=i686 -mtune=native -O3 -pipe
test:test.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $<
main:main.o regular.o
	$(CC) $^ -litpp -o $@
main.o:main.cpp regular.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o main 
