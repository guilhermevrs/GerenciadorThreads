#include "../include/uthread.h"

int main()
{
	uth_init();
	printf("antes do yield\n");
	uth_yield();
	printf("depois do yield\n");
	int a = uth_wait(2);
	printf("o wait deveria retornar -1. Ele retornou %d",a);
	return 0;
}
