The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the monobound binary search, executes two to four times faster than the standard binary search on arrays smaller than 1 million 32 bit integers.

A source code implementation in C containing every variant is available in the [binary_search.c](https://github.com/scandum/binary_search/blob/master/binary_search.c) file which also contains a bench marking routine. A graph with performance results is included at the bottom of this page. Keep in mind performance will vary depending on hardware and compiler optimizations.

I'll briefly describe each variant and notable optimizations below, followed by some performance graphs.

Deferred Detection of Equality
------------------------------

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop contains 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

Pointer Optimizations
---------------------

You can get another 10% performance boost by using pointer operations. I forgo such optimizations in the C implementation to keep things as readable as possible.

Unsigned Integer Optimization
-----------------------------

You can get a further performance boost by using unsigned instead of signed integers.

Stability
---------

All the implementations in binary_search.c should be stable. If you search an array containing the elements `[1][4][7][7][7][9]` and you search for the number `7`, it should return the right most index. This is needed if you want to use a binary search in a stable sorting algorithm. The binary search being stable shouldn't notably slow down performance.

Zero length array
-----------------

All the implementations in binary_search.c should correctly handle the case where the search function is called with 0 as the array length.

Compilation
-----------

For the monobound binary search variant to perform well the source code must be compiled with the -O1, -O2, or -O3 optimization flag. 

Standard Binary Search
----------------------

The standard binary search found in most text books.
```c
int standard_binary_search(int *array, unsigned int array_size, int key)
{
        int bot, mid, top;

        if (array_size == 0)
                return -1;

        bot = 0;
        top = array_size - 1;

        while (bot < top)
        {
                mid = top - (top - bot) / 2;

                if (key < array[mid])
                        top = mid - 1;
                else
                        bot = mid;
        }

        if (key == array[top])
                return top;
        return -1;
}
```
Boundless Binary Search
-----------------------

The boundless binary search is faster than the standard binary search since the loop contains 1 key check, 1 integer check, and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be around 20% when comparing 32 bit integers.
```c
int boundless_binary_search(int *array, unsigned int array_size, int key)
{
        unsigned int mid, bot;

        if (array_size == 0)
                return -1;

        bot = 0;
        mid = array_size;

        while (mid > 1)
        {
                if (key >= array[bot + mid / 2])
                        bot += mid++ / 2;
                mid /= 2;
        }

        if (key == array[bot])
                return bot;
        return -1;
}
```

Monobound Binary Search
-----------------------

The monobound binary search is similar to the boundless binary search but uses an extra variable to simplify calculations and performs slightly more keychecks. It's up to 60% faster than the standard binary search when comparing 32 bit integers. On small arrays the performance difference is even greater.

The performance gain is due to dynamic loop unrolling, which the traditional binary search (by trying to minimize the number of key checks) does not allow. Loop unrolling in turn allows various other potential optimizations at the compiler and cpu level.
```c
int monobound_binary_search(int *array, unsigned int array_size, int key)
{
        unsigned int bot, mid, top;

        if (array_size == 0)
                return -1;

        bot = 0;
        top = array_size;

        while (top > 1)
        {
                mid = top / 2;

                if (key >= array[bot + mid])
                        bot += mid;
                top -= mid;
        }

        if (key == array[bot])
                return bot;
        return -1;
}
```

Optimal Binary Search
--------------------------
The number of key checks can be reduced by up to 20% by not using deferred detection of equality. While this variant is slightly slower it can still be unrolled, and is likely faster for string comparisons.

```c
int optimal_binary_search(int *array, unsigned int array_size, int key)
{
        unsigned int mid, bot;
        int val;

        bot = 0;
        mid = array_size;

        while (mid)
        {
                mid /= 2;

                val = key - array[bot + mid];

                if (val == 0)
                {
                        return bot + mid;
                }
                if (val > 0)
                {
                        bot += mid + 1;
                }
        }
        return -1;
}
```
When you get to the end of a binary search and there are 3 elements left it takes 2.5 if checks to finish. The monobound binary search, however, takes 3 if checks. Subsequently the tripletapped variant performs 3 equality checks at the end with early termination, resulting in slightly fewer key checks and if the data aligns properly, slightly improved performance.

Quaternary Binary Search
----------------------------------

The dynamic unrolling of loops is often limited to 16 iterations. By narrowing down the search range by a fourth each loop, instead of a half, it takes 16 iteriations to search 4294967296 elements, instead of 65536. This optimizations slows things down slightly for smaller arrays, but can give a notable gain on larger arrays.

Monobound Interpolated Binary Search
------------------------------------

When you have an even distribution you can make an educated guess as to the location of the index. Due to the expense of the initial check and exponential search, the interpolated binary search is unlikely to outperform other binary searches on arrays with less than 1000 elements. When the distribution is uneven performance will drop, but not significantly.

A practical application for an interpolated binary search would be looking up authorization keys.

Adaptive Binary Search
----------------------

The adaptive binary search is optimized for repeated binary searches on the same array. When it observes a pattern it switches from a binary search to an exponential search. Unlike the interpolated search the adaptive search works on uneven distributions as well.

A practical application for an adaptive binary search would be accessing a unicode lookup table.

Small array benchmark graph
---------------------------
The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 binary-search.c`. Each test was ran 1,000 times with the time (in seconds) reported of the best run.

The graph below shows the execution speed on arrays with 1, 2, 4, 8, 16, 32, 64, and 128 elements on an Intel i3 quad-core processor.

![binary search graph](/binary_search_small.png)

<details><summary><b>data table</b></summary>

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                  linear_search |          1 |        806 |       9194 |      10000 |   0.000029 |
|         standard_binary_search |          1 |        806 |       9194 |      10000 |   0.000031 |
|        monobound_binary_search |          1 |        806 |       9194 |      10000 |   0.000033 |
|                                |            |            |            |            |            |
|                  linear_search |          2 |       1034 |       8966 |      19495 |   0.000039 |
|         standard_binary_search |          2 |       1034 |       8966 |      20000 |   0.000074 |
|        monobound_binary_search |          2 |       1034 |       8966 |      20000 |   0.000036 |
|                                |            |            |            |            |            |
|                  linear_search |          4 |        775 |       9225 |      38862 |   0.000046 |
|         standard_binary_search |          4 |        775 |       9225 |      30000 |   0.000122 |
|        monobound_binary_search |          4 |        775 |       9225 |      30000 |   0.000041 |
|                                |            |            |            |            |            |
|                  linear_search |          8 |        822 |       9178 |      77133 |   0.000064 |
|         standard_binary_search |          8 |        822 |       9178 |      40000 |   0.000177 |
|        monobound_binary_search |          8 |        822 |       9178 |      40000 |   0.000050 |
|                                |            |            |            |            |            |
|                  linear_search |         16 |       1141 |       8859 |     151154 |   0.000116 |
|         standard_binary_search |         16 |       1141 |       8859 |      50000 |   0.000219 |
|        monobound_binary_search |         16 |       1141 |       8859 |      50000 |   0.000064 |
|                                |            |            |            |            |            |
|                  linear_search |         32 |       1145 |       8855 |     302324 |   0.000218 |
|         standard_binary_search |         32 |       1145 |       8855 |      60000 |   0.000270 |
|        monobound_binary_search |         32 |       1145 |       8855 |      60000 |   0.000074 |
|                                |            |            |            |            |            |
|                  linear_search |         64 |       1096 |       8904 |     605248 |   0.000409 |
|         standard_binary_search |         64 |       1096 |       8904 |      70000 |   0.000321 |
|        monobound_binary_search |         64 |       1096 |       8904 |      70000 |   0.000084 |
|                                |            |            |            |            |            |
|                  linear_search |        128 |       1046 |       8954 |    1214120 |   0.000749 |
|         standard_binary_search |        128 |       1046 |       8954 |      80000 |   0.000386 |
|        monobound_binary_search |        128 |       1046 |       8954 |      80000 |   0.000097 |

</details>

Large array benchmark graph
---------------------------
The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 binary-search.c`. Each test was ran 10,000 times with the time (in seconds) reported of the best run.

The graph below shows the execution speed on arrays with 10, 100, 1000, 10000, 100000, and 1000000 elements on an Intel i3 quad-core processor.

![binary search graph](/binary_search.png)

<details><summary><b>data table</b></summary>

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |         10 |        910 |       9090 |      43646 |   0.000182 |
|        boundless_binary_search |         10 |        910 |       9090 |      43646 |   0.000156 |
|        monobound_binary_search |         10 |        910 |       9090 |      50000 |   0.000060 |
|  monobound_interpolated_search |         10 |        910 |       9090 |      64027 |   0.000203 |
|                                |            |            |            |            |            |
|         standard_binary_search |        100 |       1047 |       8953 |      77085 |   0.000361 |
|        boundless_binary_search |        100 |       1047 |       8953 |      77085 |   0.000292 |
|        monobound_binary_search |        100 |       1047 |       8953 |      80000 |   0.000096 |
|  monobound_interpolated_search |        100 |       1047 |       8953 |      92421 |   0.000234 |
|                                |            |            |            |            |            |
|         standard_binary_search |       1000 |       1041 |       8959 |     109808 |   0.000610 |
|        boundless_binary_search |       1000 |       1041 |       8959 |     109808 |   0.000489 |
|        monobound_binary_search |       1000 |       1041 |       8959 |     110000 |   0.000137 |
|  monobound_interpolated_search |       1000 |       1041 |       8959 |     108509 |   0.000147 |
|                                |            |            |            |            |            |
|         standard_binary_search |      10000 |       1024 |       8976 |     143580 |   0.000804 |
|        boundless_binary_search |      10000 |       1024 |       8976 |     143580 |   0.000651 |
|        monobound_binary_search |      10000 |       1024 |       8976 |     150000 |   0.000204 |
|  monobound_interpolated_search |      10000 |       1024 |       8976 |     109353 |   0.000202 |
|                                |            |            |            |            |            |
|         standard_binary_search |     100000 |       1040 |       8960 |     176860 |   0.001087 |
|        boundless_binary_search |     100000 |       1040 |       8960 |     176860 |   0.000903 |
|        monobound_binary_search |     100000 |       1040 |       8960 |     180000 |   0.000360 |
|  monobound_interpolated_search |     100000 |       1040 |       8960 |     123144 |   0.000290 |
|                                |            |            |            |            |            |
|         standard_binary_search |    1000000 |        993 |       9007 |     209529 |   0.001570 |
|        boundless_binary_search |    1000000 |        993 |       9007 |     209529 |   0.001369 |
|        monobound_binary_search |    1000000 |        993 |       9007 |     210000 |   0.000691 |
|  monobound_interpolated_search |    1000000 |        993 |       9007 |     124870 |   0.000374 |

</details>

monobound_bsearch() vs bsearch()
--------------------------------
The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 monobound_bsearch.c`. Each test was ran 1,000 times with the time (in seconds) reported of the best run.

The graph below shows the execution speed on arrays with 10, 100, 1K, 10K, 100K, 1M, and 10M elements on an Intel i3 quad-core processor. The bsearch function is the one provided by stdlib.h.

![binary search graph](/graph3.png)

<details><summary><b>data table</b></summary>

|       Name |      Items |       Hits |     Misses |     Checks |       Time |
| ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|  monobound |         10 |        930 |       9070 |      48149 |   0.000136 |
|    bsearch |         10 |        930 |       9070 |      34677 |   0.000202 |
|            |            |            |            |            |            |
|  monobound |        100 |       1103 |       8897 |      77539 |   0.000189 |
|    bsearch |        100 |       1103 |       8897 |      66470 |   0.000410 |
|            |            |            |            |            |            |
|  monobound |       1000 |       1033 |       8967 |     107845 |   0.000265 |
|    bsearch |       1000 |       1033 |       8967 |      98703 |   0.000623 |
|            |            |            |            |            |            |
|  monobound |      10000 |       1033 |       8967 |     147232 |   0.000357 |
|    bsearch |      10000 |       1033 |       8967 |     132342 |   0.000820 |
|            |            |            |            |            |            |
|  monobound |     100000 |       1014 |       8986 |     177576 |   0.000539 |
|    bsearch |     100000 |       1014 |       8986 |     165785 |   0.001111 |
|            |            |            |            |            |            |
|  monobound |    1000000 |        998 |       9002 |     207938 |   0.001124 |
|    bsearch |    1000000 |        998 |       9002 |     198443 |   0.001603 |
|            |            |            |            |            |            |
|  monobound |   10000000 |        974 |       9026 |     247324 |   0.002641 |
|    bsearch |   10000000 |        974 |       9026 |     232174 |   0.003784 |

</details>
