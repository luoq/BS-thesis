#CC=ifort
#FLAGS=-fpp -O3
CC=gfortran
FLAGS=-cpp -O3
all:Per test select_and_cal
select_and_cal:select_and_cal.f90 misc.o timw.o permanent.o
	$(CC) $(FLAGS) -o $@ $^
Per:Per.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
test:test.f90 misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
permanent.o:permanent.f90 misc.o 
	$(CC) $(FLAGS) -c $<
misc.o:misc.f90
	$(CC) $(FLAGS) -c $<
timw.o:timw.c
	gcc -pg -c timw.c
clean:
	rm Per test *.o *.mod
