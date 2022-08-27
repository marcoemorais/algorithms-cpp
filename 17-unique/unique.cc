#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// unique removes adjacent duplicates from sorted sequence in [first, last).
template <typename FwdIter>
FwdIter
unique(FwdIter first, FwdIter last)
{
    // p1 will point to the last unique element.
    FwdIter p1 = first, p2 = first + 1;
    while (p2 != last) {
        if (*p1 != *p2) {
            ++p1;
            if (*p1 != *p2) {
                // p2 is unique, overwrite duplicate p1.
                std::iter_swap(p1, p2);
            }
        }
        ++p2;
    }
    ++p1; // One past last unique element.

    return p1;
}

}

TEST_CASE("[unique]")
{
    namespace alg = algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::vector<int> expected;
        std::size_t expected_sz;
    };

    std::vector<test_case> test_cases{
        {
            "no duplicate values",
            {1, 2, 3, 4, 5},
            {1, 2, 3, 4, 5}, 5,
        },
        {
            "1 duplicate pair -- first",
            {1, 1, 2, 3, 4},
            {1, 2, 3, 4, 1}, 4,
        },
        {
            "1 duplicate pair -- middle",
            {1, 2, 3, 3, 4},
            {1, 2, 3, 4, 3}, 4,
        },
        {
            "1 duplicate pair -- last",
            {1, 2, 3, 4, 4},
            {1, 2, 3, 4, 4}, 4,
        },
        {
            "2 duplicate pair",
            {1, 1, 2, 3, 3},
            {1, 2, 3, 1, 3}, 3,
        },
        {
            "3 duplicate values",
            {1, 1, 1, 2, 3},
            {1, 2, 3, 1, 1}, 3,
        },
        {
            "4 duplicate values",
            {1, 1, 1, 1, 2},
            {1, 2, 1, 1, 1}, 2,
        },
        {
            "all duplicate values",
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1}, 1,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        auto end = alg::unique(std::begin(input_cp), std::end(input_cp));
        REQUIRE(std::distance(std::begin(input_cp), end) == c.expected_sz);
        REQUIRE(input_cp == c.expected);
    }
}
