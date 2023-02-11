#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAXLENGTH 5
#define OK 0
#define ERROR_INPUT_FILE -1
#define ERROR_FOUND_FILE -2
#define MORE_ERROR_ELEMENTS -3
#define ZERO_ERROR_ELEMENTS -4

void usage(void);
void input(FILE*, int**, int*);
void result(int*, int*, long long int*);
void array_print(int const*, int const*);

int main(int argc, char** argv)
{
    FILE *f;
    int rc = OK;
    if (argc != 2)
        usage();
    else
    {
        char** link_argv = argv;
        link_argv++;
        f = fopen(*link_argv, "r");
        int a[MAXLENGTH];
        //
        if (!f)
        {
            fprintf(stderr, "File %s not found! %s!\n", link_argv, strerror(errno));
            fclose(f);
            rc = ERROR_FOUND_FILE;
        }
        //
		int* pa = a;
        input(f, &pa, &rc);
        fclose(f);
		printf("\n");
		switch(rc)
		{
			case OK:
			{
				long long int res = 0;
				result(a, pa, &res);
				printf("result is %lld\n", res);
				break;
			}
			case MORE_ERROR_ELEMENTS:
				printf("More than 100 items!\n");
				break;
			default:
				printf("Empty array!\n");
		}
		array_print(a, pa);
    }
    return rc;
}

void usage(void)
{
    printf("example.exe <name file>\n");
}

void input(FILE*f, int** pa, int* rc)
{
    int i = 0;
    for (; (fscanf(f, "%d", *pa) == 1) && (i <= MAXLENGTH); ++*pa, ++i)
    {
        if (i < MAXLENGTH)
        {
            if (**(pa) < 0)
                break;
        }
        else
        {
            *rc = MORE_ERROR_ELEMENTS;
        }
    }
    if (!i)
        *rc = ZERO_ERROR_ELEMENTS;
}

void result(int* pa, int* pb, long long int* sum)
{
	long long int squre = 1;
	for (; pa < pb; ++pa)
	{
		squre *= *pa;
		*sum += squre;
	}
}

void array_print(int const* pa, int const* pb)
{
	
	printf("ARRAY:\n");
	for (; pa < pb; ++pa)
		printf("%d ", *pa);
	printf("\n");
}