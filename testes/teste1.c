#include "../include/uthread.h"

/*
	Criação de thread simples
*/

void func1()
{
	printf("Thread 1\n");
	return;
}

int main()
{
	uth_init();
	int a = uth_create(func1,NULL);
	printf("Create retornou %d \n",a);
	uth_yield();
	printf("Thread criadora\n");
	return 0;
}
