all: teste1 teste2
	echo "foi"

teste1:
	gcc -o teste1 teste1.c -L../lib/  -luthread -Wall

teste2:
	gcc -o teste2 teste2.c -L../lib/  -luthread

clean:
	rm -rf teste1 teste2
