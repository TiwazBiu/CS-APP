CC = gcc

OFLAGS = -Og
CFLAGS = $(OFLAGS) -Wall -g

CINC =
F64 =-m64

all: swap

swap: main.c swap.c
	$(CC) $(CFLAGS) $(CINC) $(F64) main.c swap.c -o swap

clean:
	rm -f *~ *.o swap