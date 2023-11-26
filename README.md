# algorithms-cpp

Some of my favorite algorithms implemented in modern c++.

## Algorithms
* [partition.cc](01-partition/partition.cc)
    * Templated partition subroutine used by many algorithms such as quicksort.
* [quicksort.cc](02-quicksort/quicksort.cc)
    * Simple implementation of quicksort.
* [rselect.cc](03-rselect/rselect.cc)
    * Finding k-th rank order statistic from random sequence in O(n).
* [toposort.cc](04-toposort/toposort.cc)
    * Topological sort implemeted using recursive depth first search.
* [dijkstra_shortest_path.cc](05-dijkstra-shortest-path/dijkstra_shortest_path.cc)
    * Dijkstra's shortest path algorithm for non-negative weight graphs.
* [bsearch.cc](06-bsearch/bsearch.cc)
    * Binary search. Simple and effective. The pizza of algorithms.
* [perms.cc](07-perms/perms.cc)
    * Return the permutations aka all possible orderings of the input.
* [powerset.cc](08-powerset/powerset.cc)
    * Return the powerset aka all possible subsets of the input.
* [bst_ops.cc](09-bst-ops/bst_ops.cc)
    * Common operations on binary search tree: search, min, max, before, after.
* [tree_ops.cc](10-tree-ops/tree_ops.cc)
    * Common operations on binary tree: height, size, lca.
* [prefix_sum.cc](11-prefix-sum/prefix_sum.cc)
    * 1d and 2d prefix sum.
* [monte_carlo_pi.cc](12-monte-carlo-pi/monte_carlo_pi.cc)
    * Monte Carlo method for approximating pi.
* [coin_change.cc](13-coin-change/coin_change.cc)
    * Find the minimum number of coins to sum to S.
* [bisection.cc](14-bisection/bisection.cc)
    * Use bisection to find the cube root of a function.
* [horners_rule.cc](15-horners-rule/horners_rule.cc)
    * Use Horner's Rule to evaluate a polynomial.
* [remove.cc](16-remove/remove.cc)
    * Remove elements matching v in [first, last) and return one past new end of range.
* [unique.cc](17-unique/unique.cc)
    * Unique removes adjacent duplicates from sorted sequence in [first, last).
* [rotate.cc](18-rotate/rotate.cc)
    * Rotate elements from [first, last) so that middle appears first.
* [combos.cc](19-combos/combos.cc)
    * Return all combinations of n objects choosing k e.g. C(n,k).
* [nqueens.cc](20-nqueens/nqueens.cc)
    * Return all unique arrangements of queens on n X n board.
* [fastpow.cc](21-fastpow/fastpow.cc)
    * Use divide-and-conquer to compute x^y.
* [rolling_hash.cc](22-rolling-hash/rolling_hash.cc)
    * Return the hash of a rolling window in O(1).
