#include "uthread.h"

void func1()
{
	uth_yield();
	printf("\nFunc1\n");
	return;
}

void func2()
{
	printf("\nFunc2\n");
	return;
}

void func3()
{
	printf("\nFunc3\n");
	return;
}

void func4()
{
	printf("\nFunc4\n");
	return;
}

int main()
{
	printf("\n\ninicio\n\n");
	uth_init();
	
	int a = uth_create(func1, NULL);
	printf("\nId da func1 eh %d\n\n", a);	

	a = uth_create(func2, NULL);
	printf("\nId da func2 eh %d\n\n", a);

	a = uth_create(func3, NULL);
	printf("\nId da func3 eh %d\n\n", a);
	
	uth_yield();
}
