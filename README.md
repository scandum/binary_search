The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the monobound binary search, executes two to four times faster than the standard binary search on arrays smaller than 1 million 32 bit integers.

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

When you get to the end of a binary search and there are 2 elements left it takes exactly 2 if checks to finish. By doing two equality checks at the end you can finish up in either 1 or 2 if checks. Subsequently, on average, the doubletapped binary search performs slightly fewer key checks.

Monobound Binary Search
-----------------------

The monobound binary search is similar to the boundless binary search but uses an extra variable to simplify calculations and performs slightly more keychecks. It's up to 60% faster than the standard binary search when comparing 32 bit integers. On small arrays the performance difference is even greater.

Tripletapped Binary Search
--------------------------

When you get to the end of a binary search and there are 3 elements left it takes 2.5 if checks to finish. The monobound binary search, however, takes 3 if checks. Subsequently the tripletapped variant performs 3 equality checks at the end with early termination, resulting in slightly fewer key checks and if the data aligns properly, slightly improved performance.

Monobound Quaternary Binary Search
----------------------------------

The monobound quaternary binary search has more key checks than the monobound binary search but in some instances runs faster on very large arrays.

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
The graph below shows the execution speed on arrays with 1, 2, 4, 8, 16, 32, 64, and 128 elements on an Intel i3 quad-core processor.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search_small.png)

Small array benchmark tables
----------------------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`. Each test was ran 100,000 times with the time (in seconds) reported of the best run.

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


Large array benchmark graph
---------------------------
The graph below shows the execution speed on arrays with 10, 100, 1000, 10000, 100000, and 1000000 elements on an Intel i3 quad-core processor.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

Large array benchmark tables
----------------------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`. Each test was ran 10,000 times with the time (in seconds) reported of the best run.

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


