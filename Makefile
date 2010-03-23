CC=g++
#CC=icc
#CXXFLAGS=-g -Dplot
CXXFLAGS=-march=i686 -mtune=native -O3 -pipe
#CXXFLAGS=-O3
all:research HPerm genRegular benchmark #test_mat test_vec test_load 

research:research.o misc.o regular.o Timer.o
	$(CC) $^ -o $@
research.o:research.cpp iSparseMatrix.h Timer.h misc.h regular.h
	$(CC) $(CXXFLAGS) -c $<
genRegular:genRegular.o regular.o Timer.o misc.o
	$(CC) $^ -o $@
genRegular.o:genRegular.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h iSparseMatrix.h misc.h
	$(CC) $(CXXFLAGS) -c $<
Timer.o:Timer.cpp Timer.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o main *.data test_vec test_mat test_load HPerm

benchmark:benchmark.o regular.o Timer.o misc.o
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

misc.o:misc.cpp misc.h
	$(CC) $(CXXFLAGS) -c $<
