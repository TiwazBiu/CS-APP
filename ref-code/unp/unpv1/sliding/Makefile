include ../Make.defines

PROGS =	client server

all:	${PROGS}

client:	client.o miscellaneous.o
		${CC} ${CFLAGS} $^ ${LIBS} -o $@

server:	server.o miscellaneous.o
		${CC} ${CFLAGS} $^ ${LIBS} -o $@
clean:
		rm -f ${PROGS} ${CLEANFILES}