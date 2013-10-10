all: tid.o listas.o uthread.o
	ar crs lib/libuthread.a bin/uthread.o bin/listas.o bin/tid.o

tid.o: src/tid.c
	gcc -o bin/tid.o -c src/tid.c

uthread.o: src/uthread.c
	gcc -o bin/uthread.o -c src/uthread.c

listas.o: src/listas.c
	gcc -o bin/listas.o -c src/listas.c

clean:
	rm  lib/*.a bin/*.o
