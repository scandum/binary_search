The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the monobound binary search, executes up to 60% faster on 32 bit integers.

A source code implementation in C is available licensed under GPL 3 (a less restrictive license may be negotiated) and is bundled with a bench marking routine. A graph with performance results is included at the bottom of this page. Keep in mind performance will vary depending on hardware and compiler optimizations.

I'll briefly describe each variant below.

Deferred Detection of Equality
------------------------------

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop contains 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

Pointer Optimizations
---------------------

You can get another 10% performance boost by using pointer operations. I forgo such optimizations in the C implementation to keep things as readable as possible.

Tailed Binary Search
--------------------

This binary search variant has the same number of key checks as the standard binary search, except that the calculations are slightly less complicated, giving it a slight performance gain without drawbacks.

Boundless Binary Search
-----------------------

The boundless binary search is significantly faster than the standard binary search since the loop contains 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be around 40-60% when comparing 32 bit integers. It performs more key checks.

Inbound Binary search
---------------------

The inbound binary search has less complex computations than the boundless binary search. Performance is similar.

Monobound Binary search
-----------------------

The monobound binary search is similar to the boundless binary search but uses an extra variable to simplify calculations. It's slightly faster than the boundless binary search.

Monobound Quaternary Binary Search
----------------------------------

The monobound quaternary binary search has more key checks than the boundless binary search but runs faster on large arrays.

Monobound Interpolated Binary Search
------------------------------------

When you have an even distribution you can make an educated guess as to the location of the index. Due to the expense of the initial check and exponential search, the interpolated binary search is unlikely to outperform other binary searches on arrays with less than 1000 elements. When the distribution is uneven performance will drop, but not significantly.

Benchmark graph
---------------
The graph below shows the execution speed on arrays with 10, 100, 1000, 10000, 100000, and 1000000 elements on an Intel i3 quad-core processor. The Y axis lists the execution time in microseconds.

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

Benchmark tables
----------------
The following benchmark was on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1). The source code was compiled using `gcc -O3 binary-search.c`

Even distribution with 10 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |         10 |    1922330 |    8077670 |   41729205 |   0.000288 |
|           tailed_binary_search |         10 |    1922330 |    8077670 |   41729205 |   0.000279 |
|        boundless_binary_search |         10 |    1922330 |    8077670 |   50000000 |   0.000166 |
|          inbound_binary_search |         10 |    1922330 |    8077670 |   50000000 |   0.000223 |
|        monobound_binary_search |         10 |    1922330 |    8077670 |   50000000 |   0.000163 |
|    monobound_quaternary_search |         10 |    1922330 |    8077670 |   50000000 |   0.000163 |
|  monobound_interpolated_search |         10 |    1922330 |    8077670 |   56147405 |   0.000273 |

Even distribution with 100 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |        100 |    1745744 |    8254256 |   76580993 |   0.000486 |
|           tailed_binary_search |        100 |    1745744 |    8254256 |   76580993 |   0.000455 |
|        boundless_binary_search |        100 |    1745744 |    8254256 |   80000000 |   0.000221 |
|          inbound_binary_search |        100 |    1745744 |    8254256 |   80000000 |   0.000261 |
|        monobound_binary_search |        100 |    1745744 |    8254256 |   80000000 |   0.000223 |
|    monobound_quaternary_search |        100 |    1745744 |    8254256 |   80000000 |   0.000219 |
|  monobound_interpolated_search |        100 |    1745744 |    8254256 |   70333100 |   0.000227 |

Even distribution with 1000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |       1000 |    1820767 |    8179233 |  109742541 |   0.000718 |
|           tailed_binary_search |       1000 |    1820767 |    8179233 |  109742541 |   0.000653 |
|        boundless_binary_search |       1000 |    1820767 |    8179233 |  110000000 |   0.000278 |
|          inbound_binary_search |       1000 |    1820767 |    8179233 |  110000000 |   0.000317 |
|        monobound_binary_search |       1000 |    1820767 |    8179233 |  110000000 |   0.000275 |
|    monobound_quaternary_search |       1000 |    1820767 |    8179233 |  112403826 |   0.000302 |
|  monobound_interpolated_search |       1000 |    1820767 |    8179233 |   82578991 |   0.000290 |

Even distribution with 10000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |      10000 |    1819226 |    8180774 |  143609833 |   0.000941 |
|           tailed_binary_search |      10000 |    1819226 |    8180774 |  143609833 |   0.000873 |
|        boundless_binary_search |      10000 |    1819226 |    8180774 |  150000000 |   0.000361 |
|          inbound_binary_search |      10000 |    1819226 |    8180774 |  150000000 |   0.000391 |
|        monobound_binary_search |      10000 |    1819226 |    8180774 |  150000000 |   0.000356 |
|    monobound_quaternary_search |      10000 |    1819226 |    8180774 |  157569839 |   0.000431 |
|  monobound_interpolated_search |      10000 |    1819226 |    8180774 |   90551566 |   0.000369 |

Even distribution with 100000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |     100000 |    1822967 |    8177033 |  176899894 |   0.001233 |
|           tailed_binary_search |     100000 |    1822967 |    8177033 |  176899894 |   0.001136 |
|        boundless_binary_search |     100000 |    1822967 |    8177033 |  180000000 |   0.000545 |
|          inbound_binary_search |     100000 |    1822967 |    8177033 |  180000000 |   0.000567 |
|        monobound_binary_search |     100000 |    1822967 |    8177033 |  180000000 |   0.000533 |
|    monobound_quaternary_search |     100000 |    1822967 |    8177033 |  192482426 |   0.000611 |
|  monobound_interpolated_search |     100000 |    1822967 |    8177033 |  145919023 |   0.000424 |

Even distribution with 1000000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |    1000000 |    1817247 |    8182753 |  209512170 |   0.001726 |
|           tailed_binary_search |    1000000 |    1817247 |    8182753 |  209512170 |   0.001614 |
|        boundless_binary_search |    1000000 |    1817247 |    8182753 |  210000000 |   0.001121 |
|          inbound_binary_search |    1000000 |    1817247 |    8182753 |  210000000 |   0.001146 |
|        monobound_binary_search |    1000000 |    1817247 |    8182753 |  210000000 |   0.001083 |
|    monobound_quaternary_search |    1000000 |    1817247 |    8182753 |  224990860 |   0.000979 |
|  monobound_interpolated_search |    1000000 |    1817247 |    8182753 |  162257103 |   0.000604 |
