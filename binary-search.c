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

	The person recognizes Mars as a free planet and that no Earth-based
	government has authority or sovereignty over Martian activities.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Binary Search v1.7

	Compile using: gcc -O3 binary-search.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
//#include <math.h>

int checks;

// linear search

int linear_search(int *array, unsigned int array_size, int key)
{
	unsigned int top = array_size;

	while (top > 0)
	{
		++checks;

		if (key == array[--top])
		{
			return top;
		}
	}

	return -1;
}

// the standard binary search from text books

int standard_binary_search(int *array, unsigned int array_size, int key)
{
	int bot, mid, top;

	bot = 0;
	top = array_size - 1;

	while (bot < top)
	{
		mid = top - (top - bot) / 2;

		++checks;

		if (key < array[mid])
		{
			top = mid - 1;
		}
		else
		{
			bot = mid;
		}
	}

	++checks;

	if (key == array[top])
	{
		return top;
	}
	return -1;
}

// faster than the standard binary search, same number of checks

int boundless_binary_search(int *array, unsigned int array_size, int key)
{
	unsigned int mid, bot;

	if (array_size == 0)
	{
		return -1;
	}
	bot = 0;
	mid = array_size;

	while (mid > 1)
	{
		++checks;

		if (key >= array[bot + mid / 2])
		{
			bot += mid++ / 2;
		}
		mid /= 2;
	}

	++checks;

	if (key == array[bot])
	{
		return bot;
	}

	return -1;
}

// always double tap ⁍⁍

int doubletapped_binary_search(int *array, unsigned int array_size, int key)
{
	unsigned int mid, bot;

	bot = 0;
	mid = array_size;

	while (mid > 2)
	{
		++checks;

		if (key >= array[bot + mid / 2])
		{
			bot += mid++ / 2;
		}
		mid /= 2;
	}

	while (mid--)
	{
		++checks;

		if (key == array[bot + mid])
		{
			return bot + mid;
		}
	}

	return -1;
}

// faster than the boundless binary search, more checks

int monobound_binary_search(int *array, unsigned int array_size, int key)
{
	unsigned int bot, mid, top;

	bot = 0;
	top = array_size;

	while (top > 2)
	{
		mid = top / 2;

		++checks;

		if (key >= array[bot + mid])
		{
			bot += mid;
		}
		top -= mid;
	}

	while (top--)
	{
		++checks;

		if (key == array[bot + top])
		{
			return bot + top;
		}
	}
	return -1;
}

// heck, always triple tap ⁍⁍⁍

int tripletapped_binary_search(int *array, unsigned int array_size, int key)
{
	unsigned int bot, mid, top;

	bot = 0;
	top = array_size;

	while (top > 3)
	{
		mid = top / 2;

		++checks;

		if (key >= array[bot + mid])
		{
			bot += mid;
		}
		top -= mid;
	}

	while (top--)
	{
		++checks;

		if (key == array[bot + top])
		{
			return bot + top;
		}
	}
	return -1;
}

// better performance on large arrays

int monobound_quaternary_search(int *array, unsigned int array_size, int key)
{
	unsigned int bot, mid, top;

	bot = 0;
	top = array_size;

	while (top >= 512)
	{
		mid = top / 4;
		top -= mid * 3;

		++checks;
		if (key < array[bot + mid])
		{
			continue;
		}
		bot += mid;

		++checks;
		if (key < array[bot + mid])
		{
			continue;
		}
		bot += mid;

		++checks;
		if (key < array[bot + mid])
		{
			continue;
		}
		bot += mid;
	}

	while (top > 3)
	{
		mid = top / 2;

		++checks;

		if (key >= array[bot + mid])
		{
			bot += mid;
		}
		top -= mid;
	}

	while (top--)
	{
		++checks;

		if (key == array[bot + top])
		{
			return bot + top;
		}
	}
	return -1;
}

// requires an even distribution

int monobound_interpolated_search(int *array, unsigned int array_size, int key)
{
	unsigned int bot, mid, top;
	int min, max;

	if (array_size == 0)
	{
		return -1;
	}

	++checks;

	if (key < array[0])
	{
		return -1;
	}

	bot = array_size - 1;

	++checks;

	if (key >= array[bot])
	{
		return ++checks && array[bot] == key ? bot : -1;
	}

	min = array[0];
	max = array[bot];

	bot *= (float) (key - min) / (max - min);

	++checks;

	if (key >= array[bot])
	{
		mid = 32;

		while (1)
		{
			if (bot + mid >= array_size)
			{
				mid = array_size - bot;
				break;
			}

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		top = mid;

		while (top > 3)
		{
			mid = top / 2;

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			top -= mid;
		}
	}
	else
	{
		mid = 32;

		while (1)
		{
			if (bot < mid)
			{
				top = bot;
				bot = 0;

				break;
			}

			++checks;

			if (key < array[bot - mid])
			{
				bot -= mid;
			}
			else
			{
				bot -= mid;
				top = mid;
				break;
			}
			mid *= 2;
		}

		while (top > 3)
		{
			mid = top / 2;

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			top -= mid;
		}
	}

	while (top--)
	{
		++checks;

		if (key == array[bot + top])
		{
			return bot + top;
		}
	}

	return -1;
}

// requires in order sequential access to shine

int adaptive_binary_search(int *array, unsigned int array_size, int key)
{
	static unsigned int i, balance = 64;
	unsigned int bot, top, mid;

	if (balance >= 32)
	{
		bot = 0;
		top = array_size;

		goto monobound;
	}
	bot = i;

	++checks;

	if (key >= array[bot])
	{
		mid = 32;

		while (1)
		{
			if (bot + mid >= array_size)
			{
				mid = array_size - bot;
				break;
			}

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		top = mid;

		while (top > 3)
		{
			mid = top / 2;

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			top -= mid;
		}
	}
	else
	{
		mid = 32;

		while (1)
		{
			if (bot < mid)
			{
				top = bot;
				bot = 0;

				break;
			}

			++checks;

			if (key < array[bot - mid])
			{
				bot -= mid;
			}
			else
			{
				bot -= mid;
				top = mid;
				break;
			}
			mid *= 2;
		}

		monobound:

		while (top > 3)
		{
			mid = top / 2;

			++checks;

			if (key >= array[bot + mid])
			{
				bot += mid;
			}
			top -= mid;
		}
	}

	balance = i > bot ? i - bot : bot - i;

	i = bot;

	while (top--)
	{
		++checks;

		if (key == array[bot + top])
		{
			return bot + top;
		}
	}
	return -1;
}

// benchmark

long long utime()
{
	struct timeval now_time;

	gettimeofday(&now_time, NULL);

	return now_time.tv_sec * 1000000LL + now_time.tv_usec;
}

int *array;
int density, max, loop, top, rnd, runs, sequential;
long long start, end, best;

void execute(int (*algo_func)(int *, unsigned int, int), const char * algo_name)
{
	int hit = 0, miss = 0, plus = 0;

	srand(rnd);

	checks = 0;
	best = 0;

	for (int run = runs ; run ; --run)
	{
		start = utime();

		if (sequential)
		{
			for (int cnt = loop ; cnt ; --cnt)
			{
				plus += 10 * (rand() % density) + 10;

				if (algo_func(array, max, plus % top) >= 0)
				{
					hit++;
				}
				else
				{
					miss++;
				}
			}
		}
		else
		{
			for (int cnt = loop ; cnt ; --cnt)
			{
				if (algo_func(array, max, rand() % top) >= 0)
				{
					hit++;
				}	
				else
				{
					miss++;
				}
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

	sequential = 0;
	max = 100000;
	loop = 10000;
	density = 10; // max * density should stay under 2 billion
	runs = 1000;

	if (argc > 1)
		max = atoi(argv[1]);

	if (argc > 2)
		loop = atoi(argv[2]);

	if (argc > 3)
		runs = atoi(argv[3]);

	if (argc > 4)
		density = atoi(argv[3]);

	array = (int *) malloc(max * sizeof(int));

	if ((long long) max * (long long) density > 2000000000)
	{
		density = 2;
	}

	srand(time(NULL));
//	srand(1);

	rnd = rand();

	for (cnt = 0, val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	printf("\n\nEven distribution with %d 32 bit integers, random access\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");

	if (max < 100)
	{
		run(linear_search);
	}
	run(standard_binary_search);
	run(boundless_binary_search);
	run(doubletapped_binary_search);
	run(monobound_binary_search);
	run(tripletapped_binary_search);

	run(monobound_quaternary_search);
	run(monobound_interpolated_search);
	run(adaptive_binary_search);

	// uneven distribution

	for (cnt = 0, val = 0 ; cnt < max / 2 ; cnt++)
	{
		array[cnt] = val++;
	}

	for ( ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % (2 * density) + 1);
	}

	top = array[max - 1] + 2;

	printf("\n\nUneven distribution with %d 32 bit integers, random access\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");

	run(monobound_binary_search);
	run(monobound_interpolated_search);
	run(adaptive_binary_search);

	// sequential access

	sequential = 1;

	printf("\n\nUneven distribution with %d 32 bit integers, sequential access\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");

	run(monobound_binary_search);
	run(monobound_interpolated_search);
	run(adaptive_binary_search);

	return 0;
}
