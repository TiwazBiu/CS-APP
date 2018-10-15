CC = gcc
CFLAGS = -Wall

all: printf0 printf0_s printf0_c printf1 printf1_s printf1_c

#####
# printf without argument
#
printf0_c: printf0.c
	$(CC) $(CFLAGS) printf0.c -c -o printf0.o

printf0: printf0.c
	$(CC) $(CFLAGS) printf0.c  -o printf0

printf0_s: printf0.c
	$(CC) $(CFLAGS) printf0.c  -o printf0_s -static

#####
# printf with simple argument
#
printf1_c: printf1.c
	$(CC) $(CFLAGS) printf1.c -c -o printf1.o

printf1: printf1.c
	$(CC) $(CFLAGS) printf1.c  -o printf1

printf1_s: printf1.c
	$(CC) $(CFLAGS) printf1.c  -o printf1_s -static

clean:
	rm -f *~ printf[01] *.o *_s 
