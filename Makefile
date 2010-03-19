CC=g++
#CC=icpc
CXXFLAGS=-g
#CXXFLAGS=-march=i686 -mtune=native -O3 -pipe
#CXXFLAGS=-O3
test_vec:test_vec.cpp regular.cpp iSparseMatrix.h
	$(CC) $(CXXFLAGS) -o $@ $< regular.cpp

main:main.o regular.o Timer.o
	$(CC) $^ -o $@
main.o:main.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h iSparseMatrix.h
	$(CC) $(CXXFLAGS) -c $<
Timer.o:Timer.cpp Timer.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o main data test_vec

benchmark:benchmark.o regular.o Timer.o
	$(CC) $^ -o $@
benchmark.o:benchmark.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
