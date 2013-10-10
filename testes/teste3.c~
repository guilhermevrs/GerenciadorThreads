#include "../include/uthread.h"

/*
	Passagem de parametro
*/

void func1(int paramThread)
{
	printf("Numero recebido: %d\n",paramThread);
}

int main()
{
	uth_init();
	int a = uth_create(func1,400);
	printf("Create retornou %d \n",a);
	uth_yield();
	printf("Thread criadora\n");
	return 0;
}
