#CC=ifort
#FLAGS=-fpp -O3
CC=gfortran
FLAGS=-xf95-cpp-input -O3
all:Per test
Per:permanent.o Per.o misc.o timw.o
	$(CC) -o $@ $^
test:misc.o test.o timw.o
	$(CC) -o $@ $^
Per.o:Per.f90 permanent.mod gen_subsets.mod
	$(CC) $(FLAGS) -c $<
test.o:test.f90 gen_subsets.mod
	$(CC) $(FLAGS) -c $<
permanent.o:permanent.f90 gen_subsets.mod
	$(CC) $(FLAGS) -c $<
misc.o:misc.f90
	$(CC) $(FLAGS) -c $<
permanent.mod:permanent.o
gen_subsets.mod:misc.o
timw.o:timw.c
	gcc -c timw.c
clean:
	rm Per *.o *.mod
