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
	Binary Search v1.3
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

int checks;


// This is the standard binary search from text books

int standard_binary_search(int *array, int array_size, int key)
{
	register int bot, mid, i;

	bot = 0;
	i = array_size - 1;

	while (bot != i)
	{
		mid = i - (i - bot) / 2;

		if (++checks && key < array[mid])
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

// faster than the standard binary search with the same number of checks

int tailed_binary_search(int *array, int array_size, int key)
{
	register int i, mid, bot;

	i = array_size - 1;
	mid = i / 2;
	bot = 0;

	while (mid)
	{
		if (++checks && key < array[i - mid])
		{
			i -= mid + 1;
		}
		else
		{
			bot = i - mid;
		}
		mid = (i - bot) / 2;
	}

	if (i > bot && ++checks && key < array[i])
	{
		--i;
	}

	++checks;

	if (i >= 0 && key == array[i])
	{
		return i;
	}
	return -1;
}

// slightly more key checks with simpler calculations

int inbound_binary_search(int *array, int array_size, int key)
{
	register int mid, i, median;

	median = mid = array_size / 2;

	++checks;

	if (key < array[median])
	{
		i = 0;

		while (mid)
		{
			mid /= 2;

			++checks;
			if (key > array[i + mid])
			{
				i += mid + 1;

				if (i == median)
				{
					return -1;
				}
			}
		}
	}
	else
	{
		i = array_size - 1;

		while (mid)
		{
			mid /= 2;

			++checks;

			if (key < array[i - mid])
			{
				i -= mid + 1;

				if (i == median)
				{
					break;
				}
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

// more key checks but simpler calculations

int boundless_binary_search(int *array, int array_size, int key)
{
	register int mid, i;

	mid = i = array_size - 1;

	if (key < array[0])
	{
		return -1;
	}

	while (mid > 3)
	{
		mid /= 2;

		if (++checks && key < array[i - mid])
		{
			i -= mid;
		}
	}

	while (++checks && key < array[i])
	{
		--i;
	}

	if (++checks && key == array[i])
	{
		return i;
	}
	return -1;
}

// more key checks but faster execution

int boundless_quaternary_search(int *array, int array_size, int key)
{
	register int mid, i;

	++checks;

	if (key < array[0])
	{
		return -1;
	}

	mid = i = array_size - 1;

	while (mid > 7)
	{
		mid /= 4;

		++checks;

		if (key < array[i - mid])
		{
			i -= mid;

			++checks;

			if (key < array[i - mid])
			{
				i -= mid;

				++checks;

				if (key < array[i - mid])
				{
					i -= mid;
				}
			}
		}
	}

	while (++checks && key < array[i])
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


// requires an even distribution

int interpolated_search(int *array, int array_size, int key)
{
	register int mid, i, min, max;

	if (++checks && key <= (min = array[0]))
	{
		return ++checks && key == min ? 0 : -1;
	}

	i = array_size - 1;

	if (++checks && key >= (max = array[i]))
	{
		return ++checks && key == max ? i : -1;
	}

	i *= (float) (key - min) / (max - min);

	if (++checks && key >= array[i])
	{
		max = array_size - 1;

		mid = 4;

		while (1)
		{
			if (i + mid > max)
			{
				mid = max - i;
				break;
			}

			if (++checks && key > array[i + mid])
			{
				i += mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		while (mid)
		{
			mid /= 2;

			if (++checks && key > array[i + mid])
			{
				i += mid;
			}
		}
		if (++checks && key > array[i])
		{
			++i;
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

			if (++checks && key < array[i - mid])
			{
				i -= mid;
			}
			else
			{
				break;
			}
			mid *= 2;
		}

		while (mid)
		{
			mid /= 2;

			if (++checks && key < array[i - mid])
			{
				i -= mid;
			}
		}
		if (++checks && key < array[i])
		{
			--i;
		}
	}

	if (++checks && key == array[i])
	{
		return i;
	}
	return -1;
}

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
	printf(" %d hits %d misses %9d checks in %f seconds (%s)\n", hit, miss, checks, best / 1000000.0, algo_name);
}

#define run(algo) execute(&algo, #algo)

int main(int argc, char **argv)
{
	int cnt, val;

	max = 100000;
	loop = 10000;
	density = 100; // max * density should stay under 2 billion
	runs = 100;

	if (argc > 1)
		max = atoi(argv[1]);

	if (argc > 2)
		loop = atoi(argv[2]);

	if (argc > 3)
		runs = atoi(argv[3]);

	if (argc > 4)
		density = atoi(argv[3]);

	array = malloc(max * sizeof(int));

	srand(time(NULL));

	rnd = rand();

	for (cnt = 0, val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	printf("\n\nEven distribution with %d 32 bit integers\n\n", max);

	// Trial run

	run(standard_binary_search);

	// Begin

	run(standard_binary_search);
	run(tailed_binary_search);
	run(inbound_binary_search);
	run(boundless_binary_search);
	run(boundless_quaternary_search);
	run(interpolated_search);

	return 0;
}
