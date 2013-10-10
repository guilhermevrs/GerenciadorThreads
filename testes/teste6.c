#include "../include/uthread.h"

void func2()
{
	printf("thread 2\n");
}

void func1()
{
	printf("thread 1\n");
	uth_create(func2,NULL);
	printf("yield da 1\n");
	uth_yield();
	printf("retorno da 1\n");
}

int main()
{
	uth_init();
	uth_create(func1,NULL);
	printf("primeiro da main yield\n");
	uth_yield();
	printf("segundo da main yield\n");
	uth_yield();
	printf("FIM");
	return 0;
}
