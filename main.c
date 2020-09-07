#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cmpfunc(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int imax(int a, int b)
{
	return a > b ? a : b;
}

int imin(int a, int b)
{
	return a < b ? a : b;
}

struct Stemplot
{
	size_t dataLen;
	int* data;
};

struct Stemplot createStemplot(int* data, size_t dataLen)
{
	struct Stemplot s;
	s.dataLen = dataLen;
	if ((s.data = malloc(dataLen * sizeof(int))) == NULL)
	{
		fprintf(stderr, "failed malloc-ing memory for stemplot!\n");
	}
	else
	{
		memcpy(s.data, data, dataLen * sizeof(int));
		qsort(s.data, dataLen, sizeof(int), cmpfunc);
	}

	return s;
}

void printStemplot(struct Stemplot* s)
{
	if (s->dataLen == 0) return;

	int plotMin = s->data[0];
	int plotMax = plotMin;
	for (int i = 1; i < s->dataLen; ++i)
	{
		int point = s->data[i];
		plotMin = imin(plotMin, point);
		plotMax = imax(plotMax, point);
	}
	plotMin /= 10;
	plotMax /= 10;

	for (int stem = plotMin; stem <= plotMax; ++stem)
	{
		printf("% 2d| ", stem);
		for (size_t i = 0; i < s->dataLen; ++i)
		{
			int val = s->data[i];
			int valStem = val / 10;
			if (valStem == stem)
			{
				printf("%d ", val - valStem * 10);
			}
		}
		printf("\n");
	}
}

void freeStemplot(struct Stemplot *s)
{
	free(s->data);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Requires a list input as an argument\n");
		return 1;
	}
	size_t dataLen = (size_t)argc - 1;
	int* data = malloc(dataLen * sizeof(int));
	if (data == NULL)
	{
		fprintf(stderr, "unable to allocate memory for data array\n");
		return 1;
	}
	for (size_t i = 0; i < dataLen; ++i)
	{
		data[i] = atoi(argv[i]);
	}
	struct Stemplot plot = createStemplot(data, dataLen);

	printStemplot(&plot);

	freeStemplot(&plot);
	return 0;
}
