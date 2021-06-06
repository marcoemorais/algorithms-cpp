#include <algorithm>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// partition elements and return pointer to start of second group.
template <typename BidirIt,
          typename UnaryPredicate>
BidirIt
partition(BidirIt first, BidirIt last, UnaryPredicate pred)
{
    BidirIt hi = first, unpartitioned = first;
    while (unpartitioned != last) {
        if (pred(*unpartitioned)) {
            // The unpartitioned element is less than the start of second
            // group.  Swap with current start of second group to restore
            // invariant.
            std::swap(*unpartitioned, *hi);
            ++hi;
        }
        ++unpartitioned;
    }
    return hi;
}

// partition elements and return pointer to first element where !(x < v).
template <typename BidirIt,
          typename T = typename std::iterator_traits<BidirIt>::value_type>
BidirIt
partition_less(BidirIt first, BidirIt last, const T& v)
{
    auto less = [&v](const T& x) { return x < v; };
    return algorithms::partition(first, last, less);
}

}

TEST_CASE("[partition]")
{
    using namespace algorithms;

    // PartitionResult holds expected arrangement after partitioning.
    using PartitionResult = std::tuple<int, int, std::vector<int>>;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::vector<PartitionResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Sorted ascending.",
            {1, 3, 5, 7, 9},
            {
                {1, 1, {1, 3, 5, 7, 9}},
                {2, 3, {1, 3, 5, 7, 9}},
                {3, 3, {1, 3, 5, 7, 9}},
                {4, 5, {1, 3, 5, 7, 9}},
                {5, 5, {1, 3, 5, 7, 9}},
                {6, 7, {1, 3, 5, 7, 9}},
                {7, 7, {1, 3, 5, 7, 9}},
                {8, 9, {1, 3, 5, 7, 9}},
                {9, 9, {1, 3, 5, 7, 9}},
            }
        },
        {
            "Sorted descending.",
            {9, 7, 5, 3, 1},
            {
                {1, 9, {9, 7, 5, 3, 1}},
                {2, 7, {1, 7, 5, 3, 9}},
                {3, 7, {1, 7, 5, 3, 9}},
                {4, 5, {3, 1, 5, 9, 7}},
                {5, 5, {3, 1, 5, 9, 7}},
                {6, 7, {5, 3, 1, 7, 9}},
                {7, 7, {5, 3, 1, 7, 9}},
                {8, 9, {7, 5, 3, 1, 9}},
                {9, 9, {7, 5, 3, 1, 9}},
            }
        },
        {
            "Random.",
            {3, 7, 1, 9, 5},
            {
                {1, 3, {3, 7, 1, 9, 5}},
                {2, 7, {1, 7, 3, 9, 5}},
                {3, 7, {1, 7, 3, 9, 5}},
                {4, 7, {3, 1, 7, 9, 5}},
                {5, 7, {3, 1, 7, 9, 5}},
                {6, 9, {3, 1, 5, 9, 7}},
                {7, 9, {3, 1, 5, 9, 7}},
                {8, 9, {3, 7, 1, 5, 9}},
                {9, 9, {3, 7, 1, 5, 9}},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        for (const auto& expect : c.expected) {
            auto input_cp = c.input;
            auto v = std::get<0>(expect);
            auto rcv = partition_less(begin(input_cp), end(input_cp), v);
            REQUIRE(*rcv == std::get<1>(expect));
            REQUIRE(input_cp == std::get<2>(expect));
        }
    }
}
