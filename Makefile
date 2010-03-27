CC=g++
#CXXFLAGS=-g -Ddebug_rnw
CXXFLAGS=-march=i686 -mtune=native -O3 -pipe
all:research HPerm genRegular benchmark_HPerm benchmark_RNW test_mat test_vec test_load test_subsetGenerator test_RNW test_fmat

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
	rm *.o test_vec test_mat test_load HPerm research benchmark_HPerm genRegular test_fmat test_RNW test_subsetGenerator benchmark_RNW

benchmark_HPerm:benchmark_HPerm.o regular.o Timer.o misc.o
	$(CC) $^ -o $@
benchmark_HPerm.o:benchmark_HPerm.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
benchmark_RNW:benchmark_RNW.o regular.o Timer.o misc.o
	$(CC) $^ -o $@
benchmark_RNW.o:benchmark_RNW.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<

test_mat:test_mat.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $< misc.o regular.cpp
test_vec:test_vec.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $< misc.o regular.cpp
test_load:test_load.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $<
test_subsetGenerator:test_subsetGenerator.cpp misc.h
	$(CC) $(CXXFLAGS) -o $@ misc.o $<
test_RNW:test_RNW.cpp iSparseMatrix.h R-NW.h iFullMatrix.h misc.h
	$(CC) $(CXXFLAGS) -o $@ misc.o Timer.o $<
test_fmat:test_fmat.cpp iFullMatrix.h misc.h
	$(CC) $(CXXFLAGS) -o $@ $<

HPerm.o:HPerm.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -c $<
HPerm:HPerm.o Timer.o misc.o
	$(CC) -o $@ $^

misc.o:misc.cpp misc.h
	$(CC) $(CXXFLAGS) -c $<
