#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char **argv)
{
	char c;

	FILE *fp = stdout;


//	opterr = 0;

	while(1)
	{
		c = getopt(argc, argv, "-abcd:e");
		if (c < 0)
			break;

		switch (c)
		{
			case 1:
				printf("%s\n", argv[optind - 1]);
				break;
			case 'a':
				printf("parameter is a\n");
				break;
			case 'b':
				printf("parameter is b\n");
				break;
			case 'c':
				printf("parameter is c\n");
				break;
			case 'd':
				if (strcmp(optarg, "1") == 0)
					printf("parameter d is 1\n");
				else if (strcmp(optarg, "2") == 0)
					printf("parameter d is 2\n");
				else
					fprintf(stderr, "Invalid argument of -d\n");
				break;
			case 'e':
				printf("parameter is c\n");
				break;
			case '?':
				printf("can not resolve argument\n");
				break;
			default:
				break;

		}

	}



	return 0;

}
