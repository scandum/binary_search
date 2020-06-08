The binary search variant which is used by virtually all modern software was first published by Hermann Bottenbruch in 1962 and hasn't notably changed since.

I succeeded in finding several novel variants with improved performance. The algorithms are licensed under GPL 3 and are available as a collection that is bundled with a bench marking routine. A graph with performance results is included at the bottom of this page.

The implementations provide a stable search, meaning that if an array contains multiple duplicates the right most matching element is returned. I'll briefly describe each variant below.

**Deferred Detection of Equality**

By skipping the detection of equality until the binary search has finished (which does not allow for early termination) each loop exists of 1 key check, 1 integer check and 2 integer assignments. This is pretty much the standard algorithm that has been used since 1962.

**Tailed Binary Search**

This binary search variant has the same number of key checks as the standard binary search, except that the calculations are slightly less complicated, giving it a 1-2% performance gain without a single drawback. The tailed binary search is the best pick when key checks are expensive.

**Boundless Binary Search**

A boundless binary search is faster than the standard binary search since the loop exists of 1 key check 1 integer check and (on average) 1.5 integer assignments. The performance gain will vary depending on several factors, but should be greater than 5% when comparing 32 bit integers. It performs more key checks.

**Quaternary Boundless Binary Search**

The boundless quaternary binary search has more key checks than the boundless search but takes advantage of branch prediction. In most of my tests it gives a performance gain greater than 10% when comparing 32 bit integers.

**Inbound Binary search**

Due to boundary issues the mid - 1 optimization used in the standard search cannot easily be used in the boundless binary search. The inbound binary search solves this problem and subsequently has fewer key checks than the boundless search. While it's not as fast as the quaternary binary search it might have some utility when used on 64 bit integers and strings.

**Interpolated Binary Search**

When you have an even distribution you can make an educated guess as to the location of the index. Due to the expense of the initial check and error correction the interpolated binary search is unlikely to outperform other binary searches on arrays with less than 100 elements.

**Benchmark**

![binary search graph](https://github.com/scandum/binary_search/blob/master/binary_search.png)

The above graph shows the execution speed of 10 million searches on arrays with 10, 100, 1000, 10000 and 100000 elements on an Intel quad-core processor. The Y axis lists the execution time in milliseconds.

The tailed binary search (green) is in all cases more efficient than the standard binary search (red) while performing the same number of key checks. The graph should speak for itself.
