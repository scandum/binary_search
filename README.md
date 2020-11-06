The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the monobound binary search, executes up to 60% faster on 32 bit integers.

A source code implementation in C is available licensed under GPL 3 (a less restrictive license may be negotiated) and is bundled with a bench marking routine. A graph with performance results is included at the bottom of this page. Keep in mind performance will vary depending on hardware and compiler optimizations.

I'll briefly describe each variant below.

Deferred Detection of Equality
------------------------------

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop contains 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

Pointer Optimizations
---------------------

You can get another 10% performance boost by using pointer operations. I forgo such optimizations in the C implementation to keep things as readable as possible.

Unsigned Integer Optimization
-----------------------------

You can get a further performance boost by using unsigned instead of signed integers.

Boundless Binary Search
-----------------------

The boundless binary search is faster than the standard binary search since the loop contains 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be around 20% when comparing 32 bit integers.

Monobound Binary search
-----------------------

The monobound binary search is similar to the boundless binary search but uses an extra variable to simplify calculations and performs more keychecks. It's up to 60% faster than the standard binary search when comparing 32 bit integers.

Monobound Quaternary Binary Search
----------------------------------

The monobound quaternary binary search has more key checks than the monobound binary search but runs faster on large arrays.

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
|         standard_binary_search |         10 |    1722579 |    8277421 |   43102383 |   0.000295 |
|        boundless_binary_search |         10 |    1722579 |    8277421 |   43102383 |   0.000279 |
|        monobound_binary_search |         10 |    1722579 |    8277421 |   50000000 |   0.000159 |
|    monobound_quaternary_search |         10 |    1722579 |    8277421 |   50000000 |   0.000160 |
|  monobound_interpolated_search |         10 |    1722579 |    8277421 |   50350850 |   0.000335 |

Even distribution with 100 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |        100 |    1759908 |    8240092 |   76848523 |   0.000493 |
|        boundless_binary_search |        100 |    1759908 |    8240092 |   76848523 |   0.000446 |
|        monobound_binary_search |        100 |    1759908 |    8240092 |   80000000 |   0.000204 |
|    monobound_quaternary_search |        100 |    1759908 |    8240092 |   80000000 |   0.000203 |
|  monobound_interpolated_search |        100 |    1759908 |    8240092 |   85634669 |   0.000355 |

Even distribution with 1000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |       1000 |    1802066 |    8197934 |  109748966 |   0.000727 |
|        boundless_binary_search |       1000 |    1802066 |    8197934 |  109748966 |   0.000630 |
|        monobound_binary_search |       1000 |    1802066 |    8197934 |  110000000 |   0.000255 |
|    monobound_quaternary_search |       1000 |    1802066 |    8197934 |  112311055 |   0.000289 |
|  monobound_interpolated_search |       1000 |    1802066 |    8197934 |   98942421 |   0.000303 |

Even distribution with 10000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |      10000 |    1822576 |    8177424 |  143575407 |   0.000952 |
|        boundless_binary_search |      10000 |    1822576 |    8177424 |  143575407 |   0.000815 |
|        monobound_binary_search |      10000 |    1822576 |    8177424 |  150000000 |   0.000324 |
|    monobound_quaternary_search |      10000 |    1822576 |    8177424 |  157384779 |   0.000419 |
|  monobound_interpolated_search |      10000 |    1822576 |    8177424 |  100454815 |   0.000290 |

Even distribution with 100000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |     100000 |    1824396 |    8175604 |  176900392 |   0.001247 |
|        boundless_binary_search |     100000 |    1824396 |    8175604 |  176900392 |   0.001073 |
|        monobound_binary_search |     100000 |    1824396 |    8175604 |  180000000 |   0.000518 |
|    monobound_quaternary_search |     100000 |    1824396 |    8175604 |  190007103 |   0.000553 |
|  monobound_interpolated_search |     100000 |    1824396 |    8175604 |  100716422 |   0.000349 |

Even distribution with 1000000 32 bit integers

|                           Name |      Items |       Hits |     Misses |     Checks |       Time |
|                     ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|         standard_binary_search |    1000000 |    1819563 |    8180437 |  209513570 |   0.001738 |
|        boundless_binary_search |    1000000 |    1819563 |    8180437 |  209513570 |   0.001540 |
|        monobound_binary_search |    1000000 |    1819563 |    8180437 |  210000000 |   0.001000 |
|    monobound_quaternary_search |    1000000 |    1819563 |    8180437 |  224985387 |   0.000920 |
|  monobound_interpolated_search |    1000000 |    1819563 |    8180437 |  120151866 |   0.000586 |
