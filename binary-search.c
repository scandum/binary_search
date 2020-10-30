/*
	Copyright 2014-2020 Igor van den Hoven

	                    ivdhoven@gmail.com
*/

/*
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	Binary Search v1.5

	Compile using: gcc -O3 binary-search.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

int checks;


// This is the standard binary search from text books

int standard_binary_search(int *array, int array_size, int key)
{
	register int bot, mid, i;

	bot = 0;
	i = array_size - 1;

	while (bot < i)
	{
		mid = i - (i - bot) / 2;

		++checks;

		if (key < array[mid])
		{
			i = mid - 1;
		}
		else
		{
			bot = mid;
		}
	}

	++checks;

	if (key == array[i])
	{
		return i;
	}
	return -1;
}

// slightly faster than the standard binary search

int tailed_binary_search(int *array, int array_size, int key)
{
	register int i, mid, bot;

	bot = 0;
	i = array_size -1;
	mid = i / 2;

	while (mid)
	{
		++checks;

		if (key < array[i - mid])
		{
			i -= mid + 1;
		}
		else
		{
			bot = i - mid;
		}
		mid = (i - bot) / 2;
	}

	if (bot < i && ++checks && key < array[i])
	{
		--i;
	}

	++checks;

	if (key == array[i])
	{
		return i;
	}
	return -1;
}

int boundless_binary_search(int *array, int array_size, int key)
{
	register int mid, i;

	i = 0;
	mid = array_size;

	while (mid > 1)
	{
		++checks;

		if (key >= array[i + mid / 2])
		{
			i += mid / 2;
		}
		mid = mid / 2 + mid % 2;
	}

	++checks;

	if (key == array[i])
	{
		return i;
	}
	return -1;
}

int inbound_binary_search(int *array, int array_size, int key)
{
	register int mid, i;

	mid = array_size - array_size / 2;

	++checks;

	if (key < array[mid])
	{
		i = 0;

		while (mid > 1)
		{
			mid -= mid / 2;

			++checks;

			if (key >= array[i + mid])
			{
				i += mid;
			}
		}
	}
	else
	{
		i = array_size - 1;

		while (mid > 1)
		{
			mid -= mid / 2;

			++checks;

			if (key <= array[i - mid])
			{
				i -= mid;
			}
		}
	}

	++checks;

	if (key == array[i])
	{
		return i;
	}
	return -1;
}

int monobound_binary_search(int *array, int array_size, int key)
{
	register int bot, mid, top;

	bot = 0;
	top = array_size;

	while (top > 1)
	{
		mid = top / 2;

		++checks;

		if (key >= array[bot + mid])
		{
			bot += mid;
		}
		top -= mid;
	}

	++checks;

	if (key == array[bot])
	{
		return bot;
	}
	return -1;
}

// better performance on large arrays

int monobound_quaternary_search(int *array, int array_size, int key)
{
	register int bot, mid, top;

	bot = 0;
	top = array_size;

	while (top > 255)
	{
		mid = top >> 2;

		++checks;
		if (key >= array[bot + mid])
		{
			bot += mid;

			++checks;
			if (key >= array[bot + mid])
			{
				bot += mid;

				++checks;
				if (key >= array[bot + mid])
				{
					bot += mid;
				}
			}
		}
		top -= mid * 3;
	}

	while (top > 1)
	{
		mid = top >> 1;

		++checks;

		if (key >= array[bot + mid])
		{
			bot += mid;
		}
		top -= mid;
	}

	++checks;

	if (key == array[bot])
	{
		return bot;
	}
	return -1;
}

// requires an even distribution

int monobound_interpolated_search(int *array, int array_size, int key)
{
	register int mid, i, min, max;

	i = array_size - 1;

	if (i <= 0)
	{
		return ++checks && array[0] == key ? 0 : -1;
	}

	min = array[0];
	max = array[i];

	i *= (float) (key - min) / (max - min);

	++checks;

	if (key >= array[i])
	{
		mid = 4;

		while (1)
		{
			if (i + mid >= array_size)
			{
				mid = array_size - i;
				break;
			}

			++checks;

			if (key >= array[i + mid])
			{
				i += mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		max = mid;

		while (max > 1)
		{
			mid = max >> 1;

			++checks;

			if (key >= array[i + mid])
			{
				i += mid;
			}
			max -= mid;
		}
	}
	else
	{
		mid = 4;

		while (1)
		{
			if (i - mid < 0)
			{
				mid = i;
				break;
			}

			++checks;

			if (key <= array[i - mid])
			{
				i -= mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		max = mid;

		while (max > 1)
		{
			mid = max >> 1;

			++checks;

			if (key <= array[i - mid])
			{
				i -= mid;
			}
			max -= mid;
		}
	}

	++checks;

	if (key == array[i])
	{
		return i;
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
int density, max, loop, top, rnd, runs;
long long start, end, best;
int *array;

void execute(int (*algo_func)(int *, int, int), const char * algo_name)
{
	int hit = 0, miss = 0;

	srand(rnd);

	checks = 0;
	best = 0;

	for (int run = runs ; run ; --run)
	{
		start = utime();

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

	if (argc > 1)
		max = atoi(argv[1]);

	if (argc > 2)
		loop = atoi(argv[2]);

	if (argc > 3)
		runs = atoi(argv[3]);

	if (argc > 4)
		density = atoi(argv[3]);

	array = malloc(max * sizeof(int));

	if ((long long) max * (long long) density > 2000000000)
	{
		density = 2;
	}

//	srand(time(NULL));
	srand(1);

	rnd = rand();

	for (cnt = 0, val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	printf("\n\nEven distribution with %d 32 bit integers\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");

	run(standard_binary_search);
	run(tailed_binary_search);
	run(boundless_binary_search);
	run(inbound_binary_search);
	run(monobound_binary_search);
	run(monobound_quaternary_search);
	run(monobound_interpolated_search);

	// uneven distribution

	for (cnt = 0, val = 0 ; cnt < max / 8 ; cnt++)
	{
		array[cnt] = val++;
	}

	for ( ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	printf("\n\nUneven distribution with %d 32 bit integers\n\n", max);

	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "Name", "Items", "Hits", "Misses", "Checks", "Time");
	printf("| %30s | %10s | %10s | %10s | %10s | %10s |\n", "----------", "----------", "----------", "----------", "----------", "----------");

	run(standard_binary_search);
	run(monobound_interpolated_search);

	return 0;
}
