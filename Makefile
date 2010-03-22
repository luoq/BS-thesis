CC=g++
#CC=icc
#CXXFLAGS=-g -Wall
CXXFLAGS=-march=i686 -mtune=native -O3 -pipe -Dplot
#CXXFLAGS=-O3
all:HPerm genRegular benchmark #test_mat test_vec test_load 

genRegular:genRegular.o regular.o Timer.o
	$(CC) $^ -o $@
genRegular.o:genRegular.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h iSparseMatrix.h
	$(CC) $(CXXFLAGS) -c $<
Timer.o:Timer.cpp Timer.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o main *.data test_vec test_mat test_load HPerm

benchmark:benchmark.o regular.o Timer.o
	$(CC) $^ -o $@
benchmark.o:benchmark.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<

test_mat:test_mat.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $< regular.cpp
test_vec:test_vec.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $< regular.cpp
test_load:test_load.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $<

HPerm.o:HPerm.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -c $<
HPerm:HPerm.o Timer.o
	$(CC) -o $@ $^
