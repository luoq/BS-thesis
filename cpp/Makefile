#CC=icpc
#CXXFLAGS=-O3 -Dcolnnzs -w -pipe
#CXXFLAGS=-ipo -prof-use -Dcolnnzs -Dnonnz -w
CC=g++ 
CXXFLAGS=-O3 -pipe -Dcolnnzs -Dnoautoenlarge -Dnonnz
#CXXFLAGS=-march=i686 -O2 -Dcolnnzs
CXXFLAGS2=
all:matrix2dot genRegular select-elements select-calcu genMixRegular add-elements
#all:Per genRegular benchmark-one genRegular benchmark RNW test_fmat test_mat test_vec test_load test_subsetGenerator

select-calcu:select-calcu.cpp
	$(CC) $< $(CXXFLAGS) misc.o -o $@
matrix2dot:matrix2dot.cpp
	$(CC) $< $(CXXFLAGS) -o $@
select-elements:select-elements.o misc.o regular.o Timer.o
	$(CC) $^ $(CXXFLAGS2) -o $@
select-elements.o:select-elements.cpp permanent.h Timer.h misc.h regular.h
	$(CC) $(CXXFLAGS) -c $<
add-elements:add-elements.o misc.o regular.o Timer.o
	$(CC) $^ $(CXXFLAGS2) -o $@
add-elements.o:add-elements.cpp permanent.h Timer.h misc.h regular.h
	$(CC) $(CXXFLAGS) -c $<
genRegular:genRegular.o regular.o Timer.o misc.o
	$(CC) $^ $(CXXFLAGS2) -o $@
genRegular.o:genRegular.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
genMixRegular:genMixRegular.o regular.o Timer.o misc.o
	$(CC) $^ $(CXXFLAGS2) -o $@
genMixRegular.o:genMixRegular.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
regular.o:regular.cpp regular.h misc.h
	$(CC) $(CXXFLAGS) -Dprob -c $<
Timer.o:Timer.cpp Timer.h
	$(CC) $(CXXFLAGS) -c $<

benchmark:benchmark.o regular.o Timer.o misc.o
	$(CC) $^ $(CXXFLAGS2) -o $@
benchmark.o:benchmark.cpp regular.h Timer.h
	$(CC) $(CXXFLAGS) -c $<
benchmark-one:benchmark-one.o regular.o Timer.o misc.o
	$(CC) $^ $(CXXFLAGS2) -o $@
benchmark-one.o:benchmark-one.cpp regular.h Timer.h permanent.h
	$(CC) $(CXXFLAGS) -c $<

test_mat:test_mat.cpp regular.cpp permanent.h misc.o
	$(CC) $(CXXFLAGS) -o $@ $< misc.o regular.cpp
test_vec:test_vec.cpp regular.cpp permanent.h
	$(CC) $(CXXFLAGS) -o $@ $< misc.o regular.cpp
test_load:test_load.cpp permanent.h
	$(CC) $(CXXFLAGS) -o $@ $<
test_subsetGenerator:test_subsetGenerator.cpp misc.h
	$(CC) $(CXXFLAGS) -o $@ misc.o $<
RNW:test_RNW.cpp permanent.h iFullMatrix.h misc.h
	$(CC) $(CXXFLAGS) -o $@ misc.o Timer.o $<
test_fmat:test_fmat.cpp iFullMatrix.h misc.h
	$(CC) $(CXXFLAGS) -o $@ $<
test_misc:test_misc.cpp misc.h misc.o
	$(CC) $(CXXFLAGS) -o $@ $< misc.o


Per.o:Per.cpp permanent.h
	$(CC) $(CXXFLAGS) -c $<
Per:Per.o Timer.o misc.o
	$(CC) -o $@ $^ $(CXXFLAGS2) 
misc.o:misc.cpp misc.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm *.o test_vec test_mat test_load Per select-elements benchmark benchmark-one genRegular test_fmat RNW test_subsetGenerator test_misc 1>/dev/null 2>&1
