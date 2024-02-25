#include <cstddef>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// majority returns the majority value, if one exists, in a sequence.
template<typename FwdIter,
         typename T = typename std::iterator_traits<FwdIter>::value_type>
std::optional<T>
majority(FwdIter first, FwdIter last)
{
    // Boyer-Moore majority vote algorithm.
    //
    // majority (def.)
    // A majority value in sequence of size N appears at least N/2 times.
    //
    // Use a counting heuristic to identify the majority value.
    // Pick any value as candidate and vote +1 when a value agrees
    // with candidate and vote -1 when a value disagrees.
    // If the count for candidate drops to 0, elect new candidate.
    std::size_t count{0}, len{0};
    T candidate;
    FwdIter it = first;
    while (it != last) {
        if (count == 0) {
            candidate = *it; // Replace candidate.
        }
        count += (*it == candidate) ? +1 : -1; // Vote.
        ++it, ++len;
    }

    // A second pass is required to confirm a majority value exists.
    count = 0;
    for (it = first; it != last; ++it) {
        count += (*it == candidate);
    }
    return count > len / 2 ? candidate : std::optional<T>{};
}

}

TEST_CASE("[majority]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::optional<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "3 element input, first 2 majority",
            {1, 1, 2},
            1,
        },
        {
            "3 element input, first and last majority",
            {1, 2, 1},
            1,
        },
        {
            "3 element input, last 2 majority",
            {2, 1, 1},
            1,
        },
        {
            "3 element input, no majority",
            {1, 2, 3},
            std::nullopt,
        },
        {
            "4 element input, majority exists",
            {1, 2, 1, 1},
            1,
        },
        {
            "4 element input, no majority exists",
            {1, 2, 1, 2},
            std::nullopt,
        },
        {
            "large input, majority exists",
            {2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1},
            1,
        },
        {
            "large input, no majority exists",
            {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1},
            std::nullopt,
        },
        {
            "1 element input, always majority",
            {1},
            1,
        },
        {
            "empty input, never majority",
            {},
            std::nullopt,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        INFO(c.input);
        auto rcv = majority(std::begin(c.input), std::end(c.input));
        if (c.expected == std::nullopt) {
            REQUIRE(rcv == std::nullopt);
        }
        else {
            REQUIRE(rcv.has_value() == true);
            REQUIRE(*rcv == *(c.expected));
        }
    }
}
