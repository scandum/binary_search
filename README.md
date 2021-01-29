The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the monobound binary search, executes up to 60% faster on 32 bit integers.

A source code implementation in C is available in the [binary_search.c](https://github.com/scandum/binary_search/blob/master/binary-search.c) file which also contains a bench marking routine. A graph with performance results is included at the bottom of this page. Keep in mind performance will vary depending on hardware and compiler optimizations.

I'll briefly describe each variant and notable optimizations below.

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

All the implementations in binary_search.c should be stable. If you search an array containing the elements `[1][4][7][7][7][9]` and you search for the number `7`, it should return the right most index. This is needed if you want to use a binary search in a stable sorting algorithm, and the binary search being stable shouldn't slow down performance.

Zero length array
-----------------

All the implementations in binary_search.c should correctly handle the case where the search function is called with 0 as the array length.

Boundless Binary Search
-----------------------

The boundless binary search is faster than the standard binary search since the loop contains 1 key check, 1 integer check, and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be around 20% when comparing 32 bit integers.

Doubletapped Binary Search
--------------------------

When you get to the end of a binary search and there are 2 elements left it takes exactly 2 if checks to finish. By doing two equality checks at the end you can finish up in either 1 or 2 if checks. Subsequently, on average, the doubletapped binary search performs fewer key checks and is slightly faster than the boundless binary search.

Monobound Binary Search
-----------------------

The monobound binary search is similar to the boundless binary search but uses an extra variable to simplify calculations and performs slightly more keychecks. It's up to 60% faster than the standard binary search when comparing 32 bit integers.

Tripletapped Binary Search
--------------------------

When you get to the end of a binary search and there are 3 elements left it takes 2.5 if checks to finish. The monobound binary search, however, takes 3 if checks. Subsequently the tripletapped variant performs 3 equality checks at the end with early termination, resulting in slightly fewer key checks and improved performance.

The tripletapped binary search is as fast as a linear search for arrays up to 4 elements long, after which it becomes faster.

Monobound Quaternary Binary Search
----------------------------------

The monobound quaternary binary search has more key checks than the monobound binary search but in some instances runs faster on large arrays.

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
The graph below shows the execution speed on arrays with 1, 2, 4, 8, 16, 32, 64, and 128 elements on an Intel i3 quad-core processor. The Y axis lists the execution time in microseconds.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search_small.png)

Small array benchmark tables
----------------------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`

|                       Name | Items |    Hits |  Misses |    Checks |     Time |
|                 ---------- | ----- | ------- | ------- |  -------- | -------- |
|              linear_search |     1 | 2500887 | 7499113 |  10000000 | 0.000112 |
|     standard_binary_search |     1 | 2500887 | 7499113 |  10000000 | 0.000112 |
| tripletapped_binary_search |     1 | 2500887 | 7499113 |  10000000 | 0.000112 |
|                            |       |         |         |           |          |
|              linear_search |     2 | 1998696 | 8001304 |  19000199 | 0.000117 |
|     standard_binary_search |     2 | 1998696 | 8001304 |  20000000 | 0.000128 |
| tripletapped_binary_search |     2 | 1998696 | 8001304 |  19000199 | 0.000117 |
|                            |       |         |         |           |          |
|              linear_search |     4 | 1427508 | 8572492 |  37860377 | 0.000133 |
|     standard_binary_search |     4 | 1427508 | 8572492 |  30000000 | 0.000215 |
| tripletapped_binary_search |     4 | 1427508 | 8572492 |  29287819 | 0.000133 |
|                            |       |         |         |           |          |
|              linear_search |     8 | 1229590 | 8770410 |  75696669 | 0.000153 |
|     standard_binary_search |     8 | 1229590 | 8770410 |  40000000 | 0.000299 |
| tripletapped_binary_search |     8 | 1229590 | 8770410 |  39385331 | 0.000140 |
|                            |       |         |         |           |          |
|              linear_search |    16 | 1509178 | 8490822 | 148675961 | 0.000200 |
|     standard_binary_search |    16 | 1509178 | 8490822 |  50000000 | 0.000350 |
| tripletapped_binary_search |    16 | 1509178 | 8490822 |  49244929 | 0.000154 |
|                            |       |         |         |           |          |
|              linear_search |    32 | 1748739 | 8251261 | 292889426 | 0.000287 |
|     standard_binary_search |    32 | 1748739 | 8251261 |  60000000 | 0.000398 |
| tripletapped_binary_search |    32 | 1748739 | 8251261 |  59125996 | 0.000167 |
|                            |       |         |         |           |          |
|              linear_search |    64 | 1854727 | 8145273 | 581578784 | 0.000441 |
|     standard_binary_search |    64 | 1854727 | 8145273 |  70000000 | 0.000459 |
| tripletapped_binary_search |    64 | 1854727 | 8145273 |  69072622 | 0.000184 |
|                            |       |         |         |           |          |
|              linear_search |   128 | 1946922 | 8053078 |1156325591 | 0.000807 |
|     standard_binary_search |   128 | 1946922 | 8053078 |  80000000 | 0.000525 |
| tripletapped_binary_search |   128 | 1946922 | 8053078 |  79026121 | 0.000201 |


Large array benchmark graph
---------------------------
The graph below shows the execution speed on arrays with 10, 100, 1000, 10000, 100000, and 1000000 elements on an Intel i3 quad-core processor. The Y axis lists the execution time in microseconds.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

Large array benchmark tables
----------------------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`

Even distribution with 5 32 bit integers, random access

Even distribution with 10 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                  linear_search |         10 |    1786187 |    8213813 |   91965978 |   0.000165 |
|         standard_binary_search |         10 |    1786187 |    8213813 |   43034358 |   0.000295 |
|        boundless_binary_search |         10 |    1786187 |    8213813 |   43034358 |   0.000284 |
|     doubletapped_binary_search |         10 |    1786187 |    8213813 |   42319929 |   0.000249 |
|        monobound_binary_search |         10 |    1786187 |    8213813 |   49643139 |   0.000155 |
|     tripletapped_binary_search |         10 |    1786187 |    8213813 |   48571287 |   0.000154 |
|    monobound_quaternary_search |         10 |    1786187 |    8213813 |   48571287 |   0.000156 |
|  monobound_interpolated_search |         10 |    1786187 |    8213813 |   62133042 |   0.000345 |
|         adaptive_binary_search |         10 |    1786187 |    8213813 |   48623654 |   0.000235 |


Even distribution with 100 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |        100 |    1680382 |    8319618 |   76588952 |   0.000490 |
|        boundless_binary_search |        100 |    1680382 |    8319618 |   76588952 |   0.000455 |
|     doubletapped_binary_search |        100 |    1680382 |    8319618 |   75984640 |   0.000440 |
|        monobound_binary_search |        100 |    1680382 |    8319618 |   79394504 |   0.000195 |
|     tripletapped_binary_search |        100 |    1680382 |    8319618 |   79394504 |   0.000202 |
|    monobound_quaternary_search |        100 |    1680382 |    8319618 |   79394504 |   0.000204 |
|  monobound_interpolated_search |        100 |    1680382 |    8319618 |   89050594 |   0.000339 |
|         adaptive_binary_search |        100 |    1680382 |    8319618 |   80108500 |   0.000274 |


Even distribution with 1000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |       1000 |    1811925 |    8188075 |  109725397 |   0.000727 |
|        boundless_binary_search |       1000 |    1811925 |    8188075 |  109725397 |   0.000642 |
|     doubletapped_binary_search |       1000 |    1811925 |    8188075 |  108841311 |   0.000600 |
|        monobound_binary_search |       1000 |    1811925 |    8188075 |  109117050 |   0.000242 |
|     tripletapped_binary_search |       1000 |    1811925 |    8188075 |  109117050 |   0.000249 |
|    monobound_quaternary_search |       1000 |    1811925 |    8188075 |  111652524 |   0.000285 |
|  monobound_interpolated_search |       1000 |    1811925 |    8188075 |   98119976 |   0.000315 |
|         adaptive_binary_search |       1000 |    1811925 |    8188075 |  110222769 |   0.000262 |


Even distribution with 10000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |      10000 |    1820690 |    8179310 |  143647368 |   0.000933 |
|        boundless_binary_search |      10000 |    1820690 |    8179310 |  143647368 |   0.000826 |
|     doubletapped_binary_search |      10000 |    1820690 |    8179310 |  142901468 |   0.000810 |
|        monobound_binary_search |      10000 |    1820690 |    8179310 |  149671214 |   0.000314 |
|     tripletapped_binary_search |      10000 |    1820690 |    8179310 |  148596025 |   0.000307 |
|    monobound_quaternary_search |      10000 |    1820690 |    8179310 |  156159023 |   0.000404 |
|  monobound_interpolated_search |      10000 |    1820690 |    8179310 |  101492488 |   0.000333 |
|         adaptive_binary_search |      10000 |    1820690 |    8179310 |  148849100 |   0.000315 |


Even distribution with 100000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |     100000 |    1817839 |    8182161 |  176886639 |   0.001219 |
|        boundless_binary_search |     100000 |    1817839 |    8182161 |  176886639 |   0.001079 |
|     doubletapped_binary_search |     100000 |    1817839 |    8182161 |  176259941 |   0.001070 |
|        monobound_binary_search |     100000 |    1817839 |    8182161 |  179374342 |   0.000480 |
|     tripletapped_binary_search |     100000 |    1817839 |    8182161 |  179374342 |   0.000490 |
|    monobound_quaternary_search |     100000 |    1817839 |    8182161 |  189342122 |   0.000544 |
|  monobound_interpolated_search |     100000 |    1817839 |    8182161 |  111030910 |   0.000456 |
|         adaptive_binary_search |     100000 |    1817839 |    8182161 |  179422298 |   0.000496 |


Even distribution with 1000000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |    1000000 |    1817431 |    8182569 |  209513380 |   0.001705 |
|        boundless_binary_search |    1000000 |    1817431 |    8182569 |  209513380 |   0.001547 |
|     doubletapped_binary_search |    1000000 |    1817431 |    8182569 |  208649927 |   0.001530 |
|        monobound_binary_search |    1000000 |    1817431 |    8182569 |  209134315 |   0.000987 |
|     tripletapped_binary_search |    1000000 |    1817431 |    8182569 |  209134315 |   0.001013 |
|    monobound_quaternary_search |    1000000 |    1817431 |    8182569 |  224132221 |   0.000921 |
|  monobound_interpolated_search |    1000000 |    1817431 |    8182569 |  137975345 |   0.000545 |
|         adaptive_binary_search |    1000000 |    1817431 |    8182569 |  209141121 |   0.001037 |
