include ../Make.defines

DEPS = miscellaneous.h
PROGS =	client server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -I.

all:	${PROGS}

client:	client.o miscellaneous.o
		${CC} ${CFLAGS} -I. $^ ${LIBS} -o $@

server:	server.o miscellaneous.o
		${CC} ${CFLAGS} -I. $^ ${LIBS} -o $@
clean:
		rm -f ${PROGS} ${CLEANFILES}