#include <algorithm>
#include <cstddef>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// rselect finds k-th rank order statisic of randomly ordered sequence.
// k is number from 1..N (not zero based)
template <typename RandomIt,
          typename T=typename std::iterator_traits<RandomIt>::value_type,
          typename Less=std::less<T>>
RandomIt
rselect(RandomIt first, RandomIt last, int k, Less less=std::less<T>())
{
    auto nelem = std::distance(first, last);
    if (nelem < 2) {
        return first; // Base case.
    }

    // Pick a random element from sequence as pivot p.
    thread_local std::default_random_engine gen{std::random_device{}()};
    std::uniform_int_distribution<> dis(0, nelem-1);
    auto pivot_ind = dis(gen); // Distance from first as number.
    auto pivot = first + pivot_ind;

    // Partition the sequence around the pivot.
    std::swap(*pivot, *first); // Move pivot to first.
    RandomIt hi = first+1, unpartitioned = first+1;
    while (unpartitioned != last) {
        if (less(*unpartitioned, *first)) {
            std::swap(*unpartitioned, *hi);
            ++hi;
        }
        ++unpartitioned;
    }
    pivot = hi-1; // Just prior to start of second group.
    pivot_ind = std::distance(first, pivot);
    std::swap(*first, *pivot); // Move pivot back.

    if (k-1 < pivot_ind) {
        // If k < p, then recursively search in left subrange.
        return rselect(first, pivot, k, less);
    }
    else if (k-1 > pivot_ind) {
        // If k > p, then recursively search for k-p in right subrange.
        return rselect(pivot+1, last, k-1-pivot_ind, less);
    }
    return pivot; // Found k-th rank order.
}

}

TEST_CASE("[rselect]")
{
    using namespace algorithms;

    // RSelectResult holds result of k-th order queries.
    using RSelectResult = std::pair<int, int>;

    struct test_case
    {
        std::vector<int> input;
        std::vector<RSelectResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            // Sorted ascending input.
            {1, 2, 3, 4, 5, 6, 7},
            {
                {1, 1},
                {2, 2},
                {3, 3},
                {4, 4},
                {5, 5},
                {6, 6},
                {7, 7},
            },
        },
        {
            // Sorted descending input.
            {7, 6, 5, 4, 3, 2, 1},
            {
                {1, 1},
                {2, 2},
                {3, 3},
                {4, 4},
                {5, 5},
                {6, 6},
                {7, 7},
            },
        },
        {
            // Random input.
            {6, 5, 1, 4, 2, 7, 3},
            {
                {1, 1},
                {2, 2},
                {3, 3},
                {4, 4},
                {5, 5},
                {6, 6},
                {7, 7},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.input);
        for (const auto& expect : c.expected) {
            // Copy required since input is modified by rselect.
            auto input_cp = c.input;
            auto rcv = rselect(begin(input_cp), end(input_cp), expect.first);
            REQUIRE(*rcv == expect.second);
        }
    }
}

TEST_CASE("[rselect::random]")
{
    using namespace algorithms;

    std::default_random_engine gen{std::random_device{}()};
    int minv = 0, maxv = 127;
    std::uniform_int_distribution<int> dis(minv, maxv);
    auto fill_func = [&dis, &gen](){ return dis(gen); };

    for (auto size : {4, 8, 16, 32, 64}) {
        INFO(size);
        std::vector<int> input(size, 0);
        std::generate(std::begin(input), std::end(input), fill_func);
        std::uniform_int_distribution<int> randk(1, size);

        int nrepeat = 100;
        while (nrepeat > 0) {
            INFO(nrepeat);
            std::shuffle(std::begin(input), std::end(input), gen);
            auto input_cp = input;
            int k = randk(gen);
            auto it = rselect(begin(input_cp), end(input_cp), k);
            int rcv = *it;
            std::sort(begin(input_cp), end(input_cp));
            REQUIRE(rcv == input_cp[k-1]);
            --nrepeat;
        }
    }
}
