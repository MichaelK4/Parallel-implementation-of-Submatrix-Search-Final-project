CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lm

OBJS = main.o helper.o general.o

all: proj

proj: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o proj
	
main.o: main.c helper.c general.c general.h helper.h
	$(CC) $(CFLAGS) -c main.c -o main.o
	
helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c -o helper.o

general.o: general.c general.h helper.h
	$(CC) $(CFLAGS) -c general.c -o general.o
	
run:
	./proj	
	
	
clean:
	rm -f proj $(OBJS)
