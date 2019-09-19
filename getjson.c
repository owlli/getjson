//当前仅对json成员是string,object,array做了处理
//可能会有类似于{"test"}这样的json数据(这算不算json还未确定),此程序无法处理
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "cJSON.h"

//#define DEBUG

struct head_st {
	int no;
	struct list_head h;
}HEAD;

struct item_st
{
	int no;
	cJSON *myjson;
	char *param;
	struct list_head i;
};

FILE *IFP, *OFP;

void close_file(void)
{
	if (IFP != NULL)
	{
		fclose(IFP);
	}
	if (OFP != stdout && OFP != NULL)
	{
		fclose(OFP);
	}
}


//递归查找最后的节点
int find_last_node(cJSON *test_tmp, struct list_head *ln, char *value)
{
	struct list_head *li;
	struct item_st *tmp_st;
	int n, i;
	cJSON *arr_json;
	char *str;


	li = ln->next;
	if (li == &HEAD.h) {
		printf("parameter error! You are looking for a member with a value of %s in an object, but you forgot to write the member's name on the command line.\n", value);
		exit(1);
	}

	tmp_st = container_of(li, struct item_st, i);
	printf("tmp_st->param = %s\n", tmp_st->param);
	tmp_st->myjson = cJSON_GetObjectItem(test_tmp, tmp_st->param);
	if (tmp_st->myjson == NULL) {
		printf("can not get %s in %s\n", tmp_st->param, cJSON_PrintUnformatted(test_tmp));
		return 1;
	}

	printf("tmp_st->myjson->type = %d\n", tmp_st->myjson->type);


	//test_tmp = tmp_st->myjson;

	switch (tmp_st->myjson->type) {
		case cJSON_Array:
			n = cJSON_GetArraySize(tmp_st->myjson);

			for (i = 0; i< n; i++) {
				arr_json = cJSON_GetArrayItem(tmp_st->myjson, i);
				printf("arr_json->type = %d\n", arr_json->type);
				//这里处理情况有点复杂,因为一个json数组的元素有可能是json对象类型,也有可能是是字符或其他类型
				switch (arr_json->type) {
					case cJSON_String:
						if (tmp_st->i.next != &HEAD.h) {
							printf("parameter error! %s is an array of string,it has no subordinate objects.\n", tmp_st->param);
							close_file();
							exit(1);
						}
						str = cJSON_GetStringValue(arr_json);
						if (!str) {
							printf("can not get string in %s\n", tmp_st->param);
							close_file();
							exit(1);
						}
						printf("str = %s\n", str);
						if (!strcmp(str, value)) {
							//fprintf(OFP, "%s\n", line);
							//匹配成功,令i=n,即可跳出循环,跳出循环后i=n+1
							i = n;
						} else {
							printf("mismatch between str:%s and value:%s\n", str, value);
						}

						break;
					case cJSON_Object:
						if (!find_last_node(arr_json, &tmp_st->i, value)) {
							//匹配成功,令i=n,即可跳出循环,跳出循环后i=n+1
							i = n;
						}
						break;
					default:
						break;
				}
			}

			//这里判断一下,如果数组元素迭代完了,即i==n,说明没有匹配成功
			if (i == n)
				return 1;
			//当i==n+1时,说明匹配成功了
			if (i == n + 1)
				return 0;

			break;
		case cJSON_Object:
			return find_last_node(tmp_st->myjson, &tmp_st->i, value);

			break;	
		case cJSON_String:
			if (tmp_st->i.next != &HEAD.h) {
				printf("parameter error, %s is not last node in the linked list \n", tmp_st->param);
				close_file();
				exit(1);
			}

			str = cJSON_GetStringValue(tmp_st->myjson);
			if (!str) {
				printf("can not get string in %s\n", tmp_st->param);
				close_file();
				exit(1);
			}

			printf("str = %s\n", str);
			if (!strcmp(str, value)) {
				//fprintf(OFP, "%s\n", line);
				//	printf("%s\n", line);
				return 0;
			} else {
				printf("mismatch between str:%s and value:%s\n", str, value);
				return 1;
			}

			break;	
		case cJSON_Number:
			break;
		default:
			break;

	}

}

int main(int argc, char *argv[])
{
	int ret;
	int c;
	char *param = NULL;
	char *value = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t readcc = 0;
	int l;
	cJSON *test, *test1;
	char *str = NULL;
	int i;
	struct item_st *tmp_st;
	struct list_head *li;
	cJSON *test_tmp;
	//char *str_tmp;
	long num = 0;
	long line_num = 0;

	OFP = stdout;

#ifdef	DEBUG
	//打印所有命令行参数
	for (i=0; i<argc; i++)
		printf ("argv[%d] %s\n", i, argv[i]);
#endif

	INIT_LIST_HEAD(&HEAD.h);

	//分析命令行选项,每有一个选项就分配一个节点加入到链表中,并将选项的参数加入到这个节点的param成员中
	while ((ret = getopt(argc, argv, "-s:jaid:v:nP")) != -1) {
		switch (ret) {
			case 1:
				IFP = fopen(argv[optind - 1], "r");
				if (!IFP) {
					perror(argv[optind - 1]);
					close_file();
					exit(1);
				}
				break;
			case 's':
				tmp_st = (struct item_st *)malloc(sizeof(struct item_st));
				memset(tmp_st, 0, sizeof(struct item_st));
				tmp_st->param = argv[optind - 1];
				list_add_tail(&tmp_st->i, &HEAD.h);
				//param = argv[optind - 1];
				//printf("param = %s\n", param);
				printf("tmp_st->param = %s\n", tmp_st->param);
				break;
			case 'j':
				break;
			case 'a':
				break;
			case 'i':
				break;
			case 'd':
				OFP = fopen(argv[optind - 1], "w");
				if (!OFP) {
					perror(argv[optind - 1]);
					close_file();
					exit(1);
				}
				break;
			case 'v':
				value = argv[optind - 1];
				printf("value = %s\n", value);
				break;
			case 'n':
				break;
			case 'P':
				break;
			case ':':
				break;
			case '?':
				break;
			default:
				break;

		}


	}

	if (IFP == NULL) {
		printf("missing input file!\n");
		close_file();
		exit(1);
	}

	printf("\n");
	//	printf("start printing param and value\n");
	//	printf("param = %s\n", param);
	//	printf("value = %s\n", value);

	while ((readcc = getline(&line, &len, IFP)) != -1) {
#ifdef DEBUG
		printf("line = %s\n", line);
		printf("len = %d\n", len);
		printf("IFP = %d\n", IFP);
		printf("readcc = %ld\n", readcc);
#endif
		//解析出的一行文本的最后一个字符是回车,cJSON_Parse无法解析,所以需要把它替换为尾零
		l = strlen(line);
		line[l - 1] = '\0';

		//	printf("line = %s\n", line);

		num++;

		//printf("start parsing\n");
		//解析一行原始json数据并开辟一段空间存储,使用cjson结构体指针test指向那段空间
		test = cJSON_Parse(line);
		if (!test) { 
			printf("cjson_parse fail in %ld lines\n", num);
			close_file();
			exit(1);
		}

		//		printf("parse complete, start geting string\n");

		test_tmp = test;
		//		printf("original = %s\n", cJSON_Print(test_tmp));


		if (!find_last_node(test_tmp, &HEAD.h, value)) {
			//printf("%s\n", line);
			fprintf(OFP, "%s\n", line);
			line_num ++;
		}


		cJSON_Delete(test);
#ifdef	DEBUG
		printf("cJSON delete complete\n");
		printf("\n");
#endif

	}

	printf("\nget %ld lines json data\n", line_num);

	if (line) {
		//printf("start freeing\n");
		free(line);
		//printf("free complete\n");
	}
	close_file();


	return 0;
}
