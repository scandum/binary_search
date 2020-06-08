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
	Binary Search v1.2
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

int checks;

// This is the standard binary search from text books

int binary_search(int *array, int array_size, int key)
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
	register int mid, i, bot;

	bot = 0;
	i = array_size - 1;
	mid = i / 2;

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

// more key checks but branch prediction

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

		while (true)
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

int main(int argc, char **argv)
{
	int cnt, hit, miss, val, top, rnd;
	long long start, end;
	int *array;

	const int density = 1000;
	const int max = 100000;
	const int loop = 100000;

	if (argv[1] && *argv[1])
	{
		printf("%s\n", argv[1]);
	}

	array = malloc(max * sizeof(int));

	srand(time(NULL));

	rnd = rand();

	// init

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	checks = hit = miss = 0;

	start = utime();

	for (cnt = loop ; cnt ; --cnt)
	{
		if (binary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}

	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (standard)\n", hit, miss, checks, (end - start) / 1000000.0);

	// standard

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	top = array[max - 1] + 2;

	checks = hit = miss = 0;

	start = utime();

	for (cnt = loop ; cnt ; --cnt)
	{
		if (binary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}

	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (standard)\n", hit, miss, checks, (end - start) / 1000000.0);


	// tailed

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}
	checks = hit = miss = 0;

	start = utime();

	for (cnt = loop ; cnt ; --cnt)
	{
		if (tailed_binary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}

	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (tailed)\n", hit, miss, checks, (end - start) / 1000000.0);

	// boundless

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	start = utime();

	checks = hit = miss = 0;

	for (cnt = loop ; cnt ; --cnt)
	{
		if (boundless_binary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}
	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (boundless)\n", hit, miss, checks, (end - start) / 1000000.0);

	// quaternary

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}

	start = utime();

	checks = hit = miss = 0;

	for (cnt = loop ; cnt ; --cnt)
	{
		if (boundless_quaternary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}
	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (boundless quaternary)\n", hit, miss, checks, (end - start) / 1000000.0);


	// inbound binary

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}
	start = utime();

	checks = hit = miss = 0;

	for (cnt = loop ; cnt ; --cnt)
	{
		if (inbound_binary_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}
	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (inbound binary)\n", hit, miss, checks, (end - start) / 1000000.0);

	// interpolated

	srand(rnd);

	for (cnt = val = 0 ; cnt < max ; cnt++)
	{
		array[cnt] = (val += rand() % density + 1);
	}
	start = utime();

	checks = hit = miss = 0;

	for (cnt = loop ; cnt ; --cnt)
	{
		if (interpolated_search(array, max, rand() % top) >= 0)
		{
			hit++;
		}
		else
		{
			miss++;
		}
	}
	end = utime();

	printf("Binary search: %d hits %d misses %9d checks in %f seconds (interpolated)\n", hit, miss, checks, (end - start) / 1000000.0);

	return 0;
}
