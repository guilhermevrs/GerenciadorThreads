all: libuthread.a
	gcc -o test test.c -L../GerenciadorThread/  -luthread -Wall

libuthread.a: tid.o uthread.o listas.o
	ar crs libuthread.a tid.o uthread.o listas.o

tid.o: tid.c
	gcc -c tid.c -Wall

uthread.o: uthread.c
	gcc -c uthread.c -Wall

listas.o: listas.c
	gcc -c listas.c -Wall

clean:
	rm -rf *.a *.o test
