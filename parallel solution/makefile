CC = mpicc
CFLAGS = -Wall -Wextra -std=c99 -fopenmp
LIBS = -lm

OBJS = main.o helper.o general.o MPI_Functions.o

all: proj

proj: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o proj
	
main.o: main.c helper.c general.c MPI_Functions.c general.h helper.h MPI_Functions.h
	$(CC) $(CFLAGS) -c main.c -o main.o
	
helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c -o helper.o

general.o: general.c general.h helper.h
	$(CC) $(CFLAGS) -c general.c -o general.o
	
MPI_Functions.o: MPI_Functions.c MPI_Functions.h helper.h
		$(CC) $(CFLAGS) -c MPI_Functions.c -o MPI_Functions.o
	
	
run:
	mpiexec -np 2 ./proj	
	
	
runOn2:	
	mpiexec -np 2 -machinefile  mf  -map-by  node  ./proj
	
clean:
	rm -f proj $(OBJS)
