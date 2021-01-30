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

All the implementations in binary_search.c should be stable. If you search an array containing the elements `[1][4][7][7][7][9]` and you search for the number `7`, it should return the right most index. This is needed if you want to use a binary search in a stable sorting algorithm. The binary search being stable shouldn't notably slow down performance.

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

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                  linear_search |          1 |        810 |       9190 |      10000 |   0.000110 |
|         standard_binary_search |          1 |        810 |       9190 |      10000 |   0.000110 |
|        monobound_binary_search |          1 |        810 |       9190 |      10000 |   0.000110 |
|     tripletapped_binary_search |          1 |        810 |       9190 |      10000 |   0.000118 |
|                                |            |            |            |            |            |
|                  linear_search |          2 |       1078 |       8922 |      19461 |   0.000121 |
|         standard_binary_search |          2 |       1078 |       8922 |      20000 |   0.000188 |
|        monobound_binary_search |          2 |       1078 |       8922 |      20000 |   0.000117 |
|     tripletapped_binary_search |          2 |       1078 |       8922 |      19461 |   0.000124 |
|                                |            |            |            |            |            |
|                  linear_search |          4 |        796 |       9204 |      38761 |   0.000127 |
|         standard_binary_search |          4 |        796 |       9204 |      30000 |   0.000241 |
|        monobound_binary_search |          4 |        796 |       9204 |      30000 |   0.000120 |
|     tripletapped_binary_search |          4 |        796 |       9204 |      29609 |   0.000122 |
|                                |            |            |            |            |            |
|                  linear_search |          8 |        791 |       9209 |      77182 |   0.000146 |
|         standard_binary_search |          8 |        791 |       9209 |      40000 |   0.000299 |
|        monobound_binary_search |          8 |        791 |       9209 |      40000 |   0.000129 |
|     tripletapped_binary_search |          8 |        791 |       9209 |      39592 |   0.000131 |
|                                |            |            |            |            |            |
|                  linear_search |         16 |       1049 |       8951 |     152282 |   0.000194 |
|         standard_binary_search |         16 |       1049 |       8951 |      50000 |   0.000339 |
|        monobound_binary_search |         16 |       1049 |       8951 |      50000 |   0.000142 |
|     tripletapped_binary_search |         16 |       1049 |       8951 |      49466 |   0.000142 |
|                                |            |            |            |            |            |
|                  linear_search |         32 |       1094 |       8906 |     302680 |   0.000280 |
|         standard_binary_search |         32 |       1094 |       8906 |      60000 |   0.000391 |
|        monobound_binary_search |         32 |       1094 |       8906 |      60000 |   0.000154 |
|     tripletapped_binary_search |         32 |       1094 |       8906 |      59470 |   0.000151 |
|                                |            |            |            |            |            |
|                  linear_search |         64 |       1120 |       8880 |     603901 |   0.000445 |
|         standard_binary_search |         64 |       1120 |       8880 |      70000 |   0.000442 |
|        monobound_binary_search |         64 |       1120 |       8880 |      70000 |   0.000169 |
|     tripletapped_binary_search |         64 |       1120 |       8880 |      69419 |   0.000166 |
|                                |            |            |            |            |            |
|                  linear_search |        128 |       1047 |       8953 |    1214931 |   0.000837 |
|         standard_binary_search |        128 |       1047 |       8953 |      80000 |   0.000501 |
|        monobound_binary_search |        128 |       1047 |       8953 |      80000 |   0.000184 |
|     tripletapped_binary_search |        128 |       1047 |       8953 |      79495 |   0.000180 |


Large array benchmark graph
---------------------------
The graph below shows the execution speed on arrays with 10, 100, 1000, 10000, 100000, and 1000000 elements on an Intel i3 quad-core processor. The Y axis lists the execution time in microseconds.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

Large array benchmark tables
----------------------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`

Even distribution with 10 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |         10 |        925 |       9075 |      43639 |   0.000295 |
|        boundless_binary_search |         10 |        925 |       9075 |      43639 |   0.000278 |
|     doubletapped_binary_search |         10 |        925 |       9075 |      43253 |   0.000258 |
|        monobound_binary_search |         10 |        925 |       9075 |      50000 |   0.000133 |
|     tripletapped_binary_search |         10 |        925 |       9075 |      49238 |   0.000127 |
|    monobound_quaternary_search |         10 |        925 |       9075 |      49238 |   0.000126 |
|  monobound_interpolated_search |         10 |        925 |       9075 |      63788 |   0.000331 |
|         adaptive_binary_search |         10 |        925 |       9075 |      49238 |   0.000134 |


Even distribution with 100 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |        100 |       1052 |       8948 |      77057 |   0.000468 |
|        boundless_binary_search |        100 |       1052 |       8948 |      77057 |   0.000427 |
|     doubletapped_binary_search |        100 |       1052 |       8948 |      76662 |   0.000430 |
|        monobound_binary_search |        100 |       1052 |       8948 |      80000 |   0.000184 |
|     tripletapped_binary_search |        100 |       1052 |       8948 |      79642 |   0.000180 |
|    monobound_quaternary_search |        100 |       1052 |       8948 |      79642 |   0.000181 |
|  monobound_interpolated_search |        100 |       1052 |       8948 |      92397 |   0.000361 |
|         adaptive_binary_search |        100 |       1052 |       8948 |      80457 |   0.000258 |



Even distribution with 1000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |       1000 |        978 |       9022 |     109805 |   0.000692 |
|        boundless_binary_search |       1000 |        978 |       9022 |     109805 |   0.000616 |
|     doubletapped_binary_search |       1000 |        978 |       9022 |     109358 |   0.000597 |
|        monobound_binary_search |       1000 |        978 |       9022 |     110000 |   0.000235 |
|     tripletapped_binary_search |       1000 |        978 |       9022 |     109495 |   0.000233 |
|    monobound_quaternary_search |       1000 |        978 |       9022 |     109495 |   0.000266 |
|  monobound_interpolated_search |       1000 |        978 |       9022 |     108484 |   0.000249 |
|         adaptive_binary_search |       1000 |        978 |       9022 |     110536 |   0.000248 |


Even distribution with 10000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |      10000 |       1035 |       8965 |     143651 |   0.000915 |
|        boundless_binary_search |      10000 |       1035 |       8965 |     143651 |   0.000781 |
|     doubletapped_binary_search |      10000 |       1035 |       8965 |     143214 |   0.000813 |
|        monobound_binary_search |      10000 |       1035 |       8965 |     150000 |   0.000304 |
|     tripletapped_binary_search |      10000 |       1035 |       8965 |     149211 |   0.000286 |
|    monobound_quaternary_search |      10000 |       1035 |       8965 |     149211 |   0.000377 |
|  monobound_interpolated_search |      10000 |       1035 |       8965 |     109378 |   0.000310 |
|         adaptive_binary_search |      10000 |       1035 |       8965 |     149449 |   0.000295 |


Even distribution with 100000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |     100000 |       1025 |       8975 |     176794 |   0.001200 |
|        boundless_binary_search |     100000 |       1025 |       8975 |     176794 |   0.001032 |
|     doubletapped_binary_search |     100000 |       1025 |       8975 |     176441 |   0.001085 |
|        monobound_binary_search |     100000 |       1025 |       8975 |     180000 |   0.000466 |
|     tripletapped_binary_search |     100000 |       1025 |       8975 |     179645 |   0.000463 |
|    monobound_quaternary_search |     100000 |       1025 |       8975 |     179660 |   0.000556 |
|  monobound_interpolated_search |     100000 |       1025 |       8975 |     123273 |   0.000405 |
|         adaptive_binary_search |     100000 |       1025 |       8975 |     179691 |   0.000474 |


Even distribution with 1000000 32 bit integers, random access

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |    1000000 |        977 |       9023 |     209529 |   0.001700 |
|        boundless_binary_search |    1000000 |        977 |       9023 |     209529 |   0.001512 |
|     doubletapped_binary_search |    1000000 |        977 |       9023 |     209086 |   0.001560 |
|        monobound_binary_search |    1000000 |        977 |       9023 |     210000 |   0.000986 |
|     tripletapped_binary_search |    1000000 |        977 |       9023 |     209526 |   0.000994 |
|    monobound_quaternary_search |    1000000 |        977 |       9023 |     209513 |   0.000944 |
|  monobound_interpolated_search |    1000000 |        977 |       9023 |     124781 |   0.000504 |
|         adaptive_binary_search |    1000000 |        977 |       9023 |     209533 |   0.001024 |

