#include <stdio.h>
#include <stdlib.h>

int a = 3;
long long b = (long long)&a;
int c = 3 * 3;

static int *p = NULL;

char const * fun1()
{
	    char const a[] = "abc";
		printf("fun1 *a = %c\n", *a);
		printf("fun1 a = %s\n", a);
		printf("fun1 \"abc\" = %s\n", "abc");
		printf("fun1 \"abc\" address = %p\n", "abc");
		printf("fun1 a address = %p\n", a);
	    return a;
}

char const * fun2()
{
	    return "abc";
}

char const * fun3()
{
	    char const *a = "abc";
		printf("fun3 *a = %c\n", *a);
		printf("fun3 a = %p\n", a);
	    return  a;
}


int main()
{
	int arr[3] = {1, 2, 3};
	printf("arr[0] = %d\n", arr[0]);
	printf("arr = %p\n", arr);
	printf("arr + 1 = %p\n", arr + 1);
	printf("&arr = %p\n", &arr);
	printf("sizeof(arr) = %ld\n", sizeof(arr));
	printf("sizeof(&arr) = %ld\n", sizeof(&arr));
	int *t;

	printf("sizeof(t) = %ld\n", sizeof(t));

	printf("&a = %lld\n", (long long)&a);
	printf("&a = %p\n", &a);

	printf("b = %lld\n", b);
	printf("&b = %p\n", &b);

	//fun1();

	printf("fun1 retrn %p\n", fun1());
	printf("fun1 retrn %s\n", fun1());
	printf("fun2 retrn %p\n", fun2());
	printf("fun3 retrn %p\n", fun3());
	printf("\"abc\" retrn %p\n", "abc");


	return 0;

}
