#include "../include/uthread.h"

void printThread(int waitThread)
{
	printf("Vou esperar pelas outras\n");
	uth_wait(waitThread);
	printf("Agora e para terminar");
}

void printThreadNumber(int threadNumber)
{
	int i = 0;
	for(i=threadNumber; i<40; i+=2)
	{
		printf("Thread %d imprimiu %d\n",threadNumber, i);
		uth_yield();
	}
}

int main()
{
	int a,b;
	uth_init();
	a = uth_create(printThread,2);
	b = uth_create(printThreadNumber, a+1);
	b = uth_create(printThreadNumber, b+1);
	uth_wait(a);
	printf("\nFIM\n");
	return 0;
}
