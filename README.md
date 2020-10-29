The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since. Below I'll describe several novel variants with improved performance. The most notable variant, the boundless binary search, executes up to 60% faster on 32 bit integers.

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

A boundless binary search is faster than the standard binary search since the loop contains 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be around 40-60% when comparing 32 bit integers. It performs more key checks.

Boundless Quaternary Binary Search
----------------------------------

The boundless quaternary binary search has more key checks than the boundless binary search but runs faster on very large arrays. In most of my tests it gives a performance gain between 15% and 25% when comparing 32 bit integers.

Inbound Binary search
---------------------

Due to boundary issues the mid - 1 optimization used in the standard search cannot easily be used in the boundless binary search. The inbound binary search solves this problem and allows fewer key checks than the boundless search. It is a good choice when key checks are expensive.

Monobound Binary search
-----------------------

While the traditional binary search has two bounds, this variant has one, and deals with rounding problems while at it.

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

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |         10 |    1469106 |    8530894 |   43674988 |   0.000307 |
|                     tailed_binary_search |         10 |    1469106 |    8530894 |   43674988 |   0.000291 |
|                  boundless_binary_search |         10 |    1469106 |    8530894 |   59411880 |   0.000184 |
|                    inbound_binary_search |         10 |    1469106 |    8530894 |   50000000 |   0.000219 |
|                  monobound_binary_search |         10 |    1469106 |    8530894 |   50000000 |   0.000159 |
|              boundless_quaternary_search |         10 |    1469106 |    8530894 |   65881544 |   0.000282 |
|                inbound_quaternary_search |         10 |    1469106 |    8530894 |   52497824 |   0.000286 |
|            boundless_interpolated_search |         10 |    1469106 |    8530894 |   66908147 |   0.000284 |

Even distribution with 100 32 bit integers

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |        100 |    1893496 |    8106504 |   77253800 |   0.000498 |
|                     tailed_binary_search |        100 |    1893496 |    8106504 |   77253800 |   0.000466 |
|                  boundless_binary_search |        100 |    1893496 |    8106504 |   90000000 |   0.000236 |
|                    inbound_binary_search |        100 |    1893496 |    8106504 |   80000000 |   0.000268 |
|                  monobound_binary_search |        100 |    1893496 |    8106504 |   80000000 |   0.000221 |
|              boundless_quaternary_search |        100 |    1893496 |    8106504 |  111562642 |   0.000395 |
|                inbound_quaternary_search |        100 |    1893496 |    8106504 |   94116839 |   0.000394 |
|            boundless_interpolated_search |        100 |    1893496 |    8106504 |   81624995 |   0.000349 |

Even distribution with 1000 32 bit integers

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |       1000 |    1820787 |    8179213 |  109768180 |   0.000739 |
|                     tailed_binary_search |       1000 |    1820787 |    8179213 |  109768180 |   0.000677 |
|                  boundless_binary_search |       1000 |    1820787 |    8179213 |  119998176 |   0.000287 |
|                    inbound_binary_search |       1000 |    1820787 |    8179213 |  110000000 |   0.000309 |
|                  monobound_binary_search |       1000 |    1820787 |    8179213 |  110000000 |   0.000280 |
|              boundless_quaternary_search |       1000 |    1820787 |    8179213 |  149613336 |   0.000507 |
|                inbound_quaternary_search |       1000 |    1820787 |    8179213 |  142458213 |   0.000487 |
|            boundless_interpolated_search |       1000 |    1820787 |    8179213 |   92987646 |   0.000395 |

Even distribution with 10000 32 bit integers

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |      10000 |    1821520 |    8178480 |  143639013 |   0.000962 |
|                     tailed_binary_search |      10000 |    1821520 |    8178480 |  143639013 |   0.000889 |
|                  boundless_binary_search |      10000 |    1821520 |    8178480 |  160000000 |   0.000370 |
|                    inbound_binary_search |      10000 |    1821520 |    8178480 |  150000000 |   0.000380 |
|                  monobound_binary_search |      10000 |    1821520 |    8178480 |  150000000 |   0.000356 |
|              boundless_quaternary_search |      10000 |    1821520 |    8178480 |  186122947 |   0.000641 |
|                inbound_quaternary_search |      10000 |    1821520 |    8178480 |  175005529 |   0.000616 |
|            boundless_interpolated_search |      10000 |    1821520 |    8178480 |  125575705 |   0.000453 |

Even distribution with 100000 32 bit integers

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |     100000 |    1815402 |    8184598 |  176883539 |   0.001261 |
|                     tailed_binary_search |     100000 |    1815402 |    8184598 |  176883539 |   0.001153 |
|                  boundless_binary_search |     100000 |    1815402 |    8184598 |  190000000 |   0.000554 |
|                    inbound_binary_search |     100000 |    1815402 |    8184598 |  180000000 |   0.000550 |
|                  monobound_binary_search |     100000 |    1815402 |    8184598 |  180000000 |   0.000534 |
|              boundless_quaternary_search |     100000 |    1815402 |    8184598 |  223521630 |   0.000872 |
|                inbound_quaternary_search |     100000 |    1815402 |    8184598 |  207505579 |   0.000866 |
|            boundless_interpolated_search |     100000 |    1815402 |    8184598 |  131859455 |   0.000535 |

Even distribution with 1000000 32 bit integers

|                                     Name |      Items |       Hits |     Misses |     Checks |       Time |
|                               ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                   standard_binary_search |    1000000 |    1816076 |    8183924 |  209515871 |   0.001765 |
|                     tailed_binary_search |    1000000 |    1816076 |    8183924 |  209515871 |   0.001643 |
|                  boundless_binary_search |    1000000 |    1816076 |    8183924 |  219999982 |   0.001140 |
|                    inbound_binary_search |    1000000 |    1816076 |    8183924 |  210000000 |   0.001124 |
|                  monobound_binary_search |    1000000 |    1816076 |    8183924 |  210000000 |   0.001097 |
|              boundless_quaternary_search |    1000000 |    1816076 |    8183924 |  261009949 |   0.001326 |
|                inbound_quaternary_search |    1000000 |    1816076 |    8183924 |  253886772 |   0.001303 |
|            boundless_interpolated_search |    1000000 |    1816076 |    8183924 |  161087164 |   0.000745 |
