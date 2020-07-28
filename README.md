The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the quaternary binary search, consistently executes up to 25% faster on 32 bit integers.

A source code implementation in C is available licensed under GPL 3 (a less restrictive license may be negotiated) and is bundled with a bench marking routine. A graph with performance results is included at the bottom of this page. Keep in mind performance will vary depending on hardware and compiler optimizations.

I'll briefly describe each variant below.

Deferred Detection of Equality
------------------------------

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop contains 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

Tailed Binary Search
--------------------

This binary search variant has the same number of key checks as the standard binary search, except that the calculations are slightly less complicated, giving it a slight performance gain without drawbacks.

Boundless Binary Search
-----------------------

A boundless binary search is faster than the standard binary search since the loop contains 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be between 5% and 40% when comparing 32 bit integers. It performs more key checks.

Boundless Quaternary Binary Search
----------------------------------

The boundless quaternary binary search has more key checks than the boundless binary search but runs faster on large arrays. In most of my tests it gives a performance gain between 15% and 25% when comparing 32 bit integers.

Inbound Binary search
---------------------

Due to boundary issues the mid - 1 optimization used in the standard search cannot easily be used in the boundless binary search. The inbound binary search solves this problem and allows fewer key checks than the boundless search. It might have some utility when key checks are expensive.

Inbound Quaternary Binary search
--------------------------------
Like the boundless quaternary binary search, but with fewer key checks. Overall performance is similar.

Boundless Interpolated Binary Search
------------------------------------

When you have an even distribution you can make an educated guess as to the location of the index. Due to the expense of the initial check and error correction, the interpolated binary search is unlikely to outperform other binary searches on arrays with less than 100 elements. When the distribution is uneven performance will drop, but not significantly.

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
|         standard_binary_search |         10 |     158357 |    9841643 |   42991533 |   0.000284 |
|         standard_binary_search |         10 |     158357 |    9841643 |   42991533 |   0.000283 |
|           tailed_binary_search |         10 |     158357 |    9841643 |   42991533 |   0.000276 |
|        boundless_binary_search |         10 |     158357 |    9841643 |   58195179 |   0.000249 |
|          inbound_binary_search |         10 |     158357 |    9841643 |   50000000 |   0.000282 |
|    boundless_quaternary_search |         10 |     158357 |    9841643 |   61762418 |   0.000265 |
|      inbound_quaternary_search |         10 |     158357 |    9841643 |   55400810 |   0.000271 |
|  boundless_interpolated_search |         10 |     158357 |    9841643 |   66699093 |   0.000271 |

Even distribution with 100 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |        100 |     188541 |    9811459 |   77168228 |   0.000492 |
|         standard_binary_search |        100 |     188541 |    9811459 |   77168228 |   0.000491 |
|           tailed_binary_search |        100 |     188541 |    9811459 |   77168228 |   0.000451 |
|        boundless_binary_search |        100 |     188541 |    9811459 |   99635413 |   0.000330 |
|          inbound_binary_search |        100 |     188541 |    9811459 |   80000000 |   0.000429 |
|    boundless_quaternary_search |        100 |     188541 |    9811459 |  109846685 |   0.000383 |
|      inbound_quaternary_search |        100 |     188541 |    9811459 |   95228258 |   0.000384 |
|  boundless_interpolated_search |        100 |     188541 |    9811459 |   82681266 |   0.000310 |

Even distribution with 1000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |       1000 |     194868 |    9805132 |  109697960 |   0.000729 |
|         standard_binary_search |       1000 |     194868 |    9805132 |  109697960 |   0.000728 |
|           tailed_binary_search |       1000 |     194868 |    9805132 |  109697960 |   0.000638 |
|        boundless_binary_search |       1000 |     194868 |    9805132 |  136000769 |   0.000438 |
|          inbound_binary_search |       1000 |     194868 |    9805132 |  110000000 |   0.000592 |
|    boundless_quaternary_search |       1000 |     194868 |    9805132 |  149498206 |   0.000498 |
|      inbound_quaternary_search |       1000 |     194868 |    9805132 |  143213946 |   0.000477 |
|  boundless_interpolated_search |       1000 |     194868 |    9805132 |  101245246 |   0.000348 |

Even distribution with 10000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |      10000 |     196724 |    9803276 |  143596482 |   0.000950 |
|         standard_binary_search |      10000 |     196724 |    9803276 |  143596482 |   0.000950 |
|           tailed_binary_search |      10000 |     196724 |    9803276 |  143596482 |   0.000873 |
|        boundless_binary_search |      10000 |     196724 |    9803276 |  168770687 |   0.000480 |
|          inbound_binary_search |      10000 |     196724 |    9803276 |  150000000 |   0.000793 |
|    boundless_quaternary_search |      10000 |     196724 |    9803276 |  186344773 |   0.000628 |
|      inbound_quaternary_search |      10000 |     196724 |    9803276 |  175777209 |   0.000607 |
|  boundless_interpolated_search |      10000 |     196724 |    9803276 |  116021998 |   0.000414 |

Even distribution with 100000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |     100000 |     197875 |    9802125 |  176901757 |   0.001243 |
|         standard_binary_search |     100000 |     197875 |    9802125 |  176901757 |   0.001243 |
|           tailed_binary_search |     100000 |     197875 |    9802125 |  176901757 |   0.001139 |
|        boundless_binary_search |     100000 |     197875 |    9802125 |  200406343 |   0.000710 |
|          inbound_binary_search |     100000 |     197875 |    9802125 |  180000000 |   0.001044 |
|    boundless_quaternary_search |     100000 |     197875 |    9802125 |  223608878 |   0.000862 |
|      inbound_quaternary_search |     100000 |     197875 |    9802125 |  208351360 |   0.000854 |
|  boundless_interpolated_search |     100000 |     197875 |    9802125 |  146245804 |   0.000515 |

Even distribution with 1000000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |    1000000 |     197423 |    9802577 |  209516052 |   0.001728 |
|         standard_binary_search |    1000000 |     197423 |    9802577 |  209516052 |   0.001728 |
|           tailed_binary_search |    1000000 |     197423 |    9802577 |  209516052 |   0.001609 |
|        boundless_binary_search |    1000000 |     197423 |    9802577 |  235273893 |   0.001343 |
|          inbound_binary_search |    1000000 |     197423 |    9802577 |  210000000 |   0.001509 |
|    boundless_quaternary_search |    1000000 |     197423 |    9802577 |  261110371 |   0.001314 |
|      inbound_quaternary_search |    1000000 |     197423 |    9802577 |  254735155 |   0.001276 |
|  boundless_interpolated_search |    1000000 |     197423 |    9802577 |  158754364 |   0.000707 |
