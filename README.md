The most commonly used binary search variant was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since.

I succeeded in finding several novel variants with improved performance. The algorithms are licensed under GPL 3 and are available as a collection that is bundled with a bench marking routine. A graph with performance results is included at the bottom of this page.

The implementations provide a stable search, meaning that if an array contains multiple duplicates the right most matching element is returned. I'll briefly describe each variant below.

Deferred Detection of Equality
------------------------------

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop exists of 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

Tailed Binary Search
--------------------

This binary search variant has the same number of key checks as the standard binary search, except that the calculations are slightly less complicated, giving it a 1-2% performance gain without a single drawback. The tailed binary search is the best pick when key checks are expensive.

Boundless Binary Search
-----------------------

A boundless binary search is faster than the standard binary search since the loop exists of 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on various factors, but should be greater than 5% when comparing 32 bit integers. It performs more key checks.

Boundless Quaternary Binary Search
----------------------------------

The boundless quaternary binary search has more key checks than the boundless binary search but runs faster. In most of my tests it gives a performance gain greater than 10% when comparing 32 bit integers.

Inbound Binary search
---------------------

Due to boundary issues the mid - 1 optimization used in the standard search cannot easily be used in the boundless binary search. The inbound binary search solves this problem and subsequently has fewer key checks than the boundless search. While it's not as fast as the quaternary binary search it might have some utility when key checks are expensive.

Boundless Interpolated Binary Search
------------------------------------

When you have an even distribution you can make an educated guess as to the location of the index. Due to the expense of the initial check and error correction the interpolated binary search is unlikely to outperform other binary searches on arrays with less than 100 elements. When the distribution is uneven performance will drop quickly, so the utility of the interpolated binary search is limited.

Benchmark graph
---------------

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

The above graph shows the execution speed of 10 million searches on arrays with 10, 100, 1000, 10000 and 100000 elements on an Intel quad-core processor. The Y axis lists the execution time in milliseconds.

The tailed binary search (green) is in all cases more efficient than the standard binary search (red) while performing the same number of key checks. The graph should speak for itself.

Benchmark tables
----------------
The source code was compiled using `gcc -O3 binary-search.c`

Even distribution with 10 32 bit integers
```
4271170 checks in 0.000273 seconds (standard_binary_search)
4271170 checks in 0.000266 seconds (tailed_binary_search)
4838276 checks in 0.000283 seconds (inbound_binary_search)
4986213 checks in 0.000271 seconds (boundless_binary_search)
6257821 checks in 0.000257 seconds (boundless_quaternary_search)
7410446 checks in 0.000303 seconds (interpolated_search)
```
Even distribution with 100 32 bit integers
```
 7714954 checks in 0.000528 seconds (standard_binary_search)
 7714954 checks in 0.000499 seconds (tailed_binary_search)
 7996715 checks in 0.000473 seconds (inbound_binary_search)
 9008149 checks in 0.000462 seconds (boundless_binary_search)
10990807 checks in 0.000403 seconds (boundless_quaternary_search)
 8913905 checks in 0.000345 seconds (interpolated_search)
```
Even distribution with 1000 32 bit integers
```
10978228 checks in 0.000750 seconds (standard_binary_search)
10978228 checks in 0.000727 seconds (tailed_binary_search)
10998884 checks in 0.000691 seconds (inbound_binary_search)
12567988 checks in 0.000677 seconds (boundless_binary_search)
14971660 checks in 0.000557 seconds (boundless_quaternary_search)
 9843622 checks in 0.000457 seconds (interpolated_search)
```
Even distribution with 10000 32 bit integers
```
14362424 checks in 0.000960 seconds (standard_binary_search)
14362424 checks in 0.000940 seconds (tailed_binary_search)
14999472 checks in 0.000896 seconds (inbound_binary_search)
15879598 checks in 0.000886 seconds (boundless_binary_search)
18629780 checks in 0.000764 seconds (boundless_quaternary_search)
12435840 checks in 0.000556 seconds (interpolated_search)
```
Even distribution with 100000 32 bit integers
```
17688655 checks in 0.001256 seconds (standard_binary_search)
17688655 checks in 0.001216 seconds (tailed_binary_search)
17999976 checks in 0.001166 seconds (inbound_binary_search)
19040762 checks in 0.001161 seconds (boundless_binary_search)
22352836 checks in 0.001009 seconds (boundless_quaternary_search)
16020004 checks in 0.000690 seconds (interpolated_search)
```
Even distribution with 1000000 32 bit integers
```
20951514 checks in 0.001773 seconds (standard_binary_search)
20951514 checks in 0.001721 seconds (tailed_binary_search)
21000000 checks in 0.001668 seconds (inbound_binary_search)
22528861 checks in 0.001667 seconds (boundless_binary_search)
26109365 checks in 0.001493 seconds (boundless_quaternary_search)
19301928 checks in 0.001026 seconds (interpolated_search)
```
