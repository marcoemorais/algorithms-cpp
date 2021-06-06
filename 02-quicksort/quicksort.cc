#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// quicksort sorts elements according to Less.
template <typename BidirIt,
          typename Less>
void
quicksort(BidirIt first, BidirIt last, Less less)
{
    if (std::distance(first, last) < 2) {
        return; // Base case, nothing to sort.
    }

    // Select a pivot element p.
    // A more sophisticated implementation will choosen median from N.
    BidirIt pivot = first;

    // Partition the input into subrange with values v < p and  !(v < p).
    BidirIt hi = first+1, unpartitioned = first+1;
    while (unpartitioned != last) {
        // The unpartitioned element is less than the start of second
        // group.  Swap with current start of second group to restore
        // invariant.
        if (less(*unpartitioned, *pivot)) {
            std::swap(*unpartitioned, *hi);
            ++hi;
        }
        ++unpartitioned;
    }

    // Swap the pivot and element immediately left of hi.
    // Element represents the end of the lower half of subrange.
    BidirIt lolast = hi-1;
    std::swap(*pivot, *lolast);

    // Invoke quicksort on the subranges.
    quicksort(first, lolast, less);
    quicksort(hi, last, less);
}

// quicksort_less sorts elements in ascending order.
template <typename BidirIt,
          typename T=typename std::iterator_traits<BidirIt>::value_type>
void
quicksort_less(BidirIt first, BidirIt last)
{
    return quicksort(first, last, std::less<T>());
}

}

TEST_CASE("[quicksort]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Empty input.",
            {},
            {},
        },
        {
            "1 element input.",
            {1},
            {1},
        },
        {
            "2 element sorted.",
            {1, 2},
            {1, 2},
        },
        {
            "2 element unsorted.",
            {2, 1},
            {1, 2},
        },
        {
            "3 element sorted.",
            {1, 2, 3},
            {1, 2, 3},
        },
        {
            "3 element unsorted.",
            {2, 1, 3},
            {1, 2, 3},
        },
        {
            "3 element unsorted.",
            {3, 1, 2},
            {1, 2, 3},
        },
        {
            "Random unsorted.",
            {3, 7, 1, 9, 5},
            {1, 3, 5, 7, 9},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        quicksort_less(std::begin(input_cp), std::end(input_cp));
        REQUIRE(input_cp == c.expected);
    }
}

TEST_CASE("[quicksort::random]")
{
    using namespace algorithms;

    std::default_random_engine gen{};
    int minv = 0, maxv = 127;
    std::uniform_int_distribution<int> dis(minv, maxv);
    auto fill_func = [&dis, &gen](){ return dis(gen); };

    for (auto size : {4, 8, 16, 32, 64}) {
        INFO(size);
        std::vector<int> input(size, 0);
        std::generate(std::begin(input), std::end(input), fill_func);

        int nrepeat = 10;
        while (nrepeat > 0) {
            INFO(nrepeat);
            std::shuffle(std::begin(input), std::end(input), gen);
            quicksort_less(std::begin(input), std::end(input));
            REQUIRE(std::is_sorted(std::begin(input), std::end(input)));
            --nrepeat;
        }
    }
}
