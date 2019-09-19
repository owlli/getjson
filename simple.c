#include <stdio.h>

char * fun1()
{
	char a[] = "abc";
	printf("sizeof(a) = %ld\n", sizeof(a));
	printf("&a = %p\n", &a);
	return a;
}

char *fun2()
{
	char *a = "abc";
	printf("sizeof(a) = %ld\n", sizeof(a));
	printf("&a = %p\n", &a);
	return a;
}

int main()
{
	printf("fun1 return %p\n", fun1());
	printf("fun2 return %p\n", fun2());
	printf("\"abc\" adress is %p\n", "abc");

	return 0;
}
