/*
	Copyright (C) 2014-2021 Igor van den Hoven ivdhoven@gmail.com
*/

/*
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	v1.7

	Compile using: gcc -O3 monobound_bsearch.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int checks;

// Avoid inlining to guarantee the benchmark is fair.

__attribute__ ((noinline)) int cmp_int(const void * a, const void * b)
{
        int fa = *(int *)a;
        int fb = *(int *)b;

        checks++;

        return fa - fb;
}

// Monobound binary search using gcc's bsearch() interface. Since the
// comparison is slow there's no deferred detection of equality.

void *monobound_bsearch(const void *key, const void *array, size_t nmemb, size_t size, int (*cmp)(const void *, const void *))
{
	size_t mid, top;
	int val;
	char *piv, *base = (char *) array;

	mid = top = nmemb;

	while (mid)
	{
		mid = top / 2;

		piv = base + mid * size;

		val = cmp(key, piv);

		if (val == 0)
		{
			return piv;
		}
		if (val >= 0)
		{
			base = piv;
		}
		top -= mid;
	}
	return NULL;
}

// benchmark

long long utime()
{
	struct timeval now_time;

	gettimeofday(&now_time, NULL);

	return now_time.tv_sec * 1000000LL + now_time.tv_usec;
}

int *o_array, *r_array;
int density, max, loop, top, rnd, runs;
long long start, end, best;

void execute(void *(*algo_func)(const void *, const void *, size_t, size_t, int (*cmp)(const void *, const void *)), const char * algo_name)
{
	unsigned int cnt, hit, miss;

	srand(rnd);

	best = 0;

	for (int run = runs ; run ; --run)
	{
		hit    = 0;
		miss   = 0;
		checks = 0;

		start = utime();

		for (cnt = 0 ; cnt < loop ; cnt++)
		{
			if (algo_func(r_array + cnt, o_array, max, sizeof(int), cmp_int) != NULL)
			{
				hit++;
			}	
			else
			{
				miss++;
			}
		}
		end = utime();

		if (best == 0 || end - start < best)
		{
			best = end - start;
		}
	}

	printf("| %30s | %10d | %10d | %10d | %10d | %10f |\n", algo_name, max, hit, miss, checks, best / 1000000.0);
}

#define run(algo) execute(&algo, #algo)

int main(int argc, char **argv)
{
	int cnt, val;

	max = 100000;
	loop = 10000;
	density = 10; // max * density should stay under 2 billion
	runs = 1000;

	rnd = time(NULL);

	if (argc > 1)
		max = atoi(argv[1]);

	if (argc > 2)
		runs = atoi(argv[2]);

	if (argc > 3)
		loop = atoi(argv[3]);

	if (argc > 4)
		rnd = atoi(argv[4]);

	o_array = (int *) malloc(max * sizeof(int));
	r_array = (int *) malloc(loop * sizeof(int));

	if ((long long) max * (long long) density > 2000000000)
	{
		density = 2;
	}

	for (cnt = 0, val = 0 ; cnt < max ; cnt++)
	{
		o_array[cnt] = (val += rand() % (density * 2));
	}

	top = o_array[max - 1] + density;

	srand(rnd);

	for (cnt = 0 ; cnt < loop ; cnt++)
	{
		r_array[cnt] = rand() % top;
	}

	printf("Benchmark: array size: %d, runs: %d, repetitions: %d, seed: %d, density: %d\n\n", max, runs, loop, rnd, density);

	printf("Even distribution with %d 32 bit integers, random access\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");		

	run(monobound_bsearch);
	run(bsearch);

	free(o_array);
	free(r_array);

	return 0;
}
