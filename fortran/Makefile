#CC=ifort
#FLAGS=-fpp -O3
CC=gfortran
FLAGS=-cpp -O3
all:IDEM1 IDEM2 H1 H2 RNW test select_and_cal add_and_cal
select_and_cal:select_and_cal.f90 misc.o timw.o permanent.o
	$(CC) $(FLAGS) -o $@ $^
add_and_cal:add_and_cal.f90 misc.o timw.o permanent.o
	$(CC) $(FLAGS) -o $@ $^
Per:Per.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
H1:H1.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
H2:H2.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
IDEM1:IDEM1.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
IDEM2:IDEM2.f90 permanent.o  misc.o timw.o
	$(CC) $(FLAGS) -o $@ $^
RNW:RNW.f90 permanent.o  misc.o timw.o
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
