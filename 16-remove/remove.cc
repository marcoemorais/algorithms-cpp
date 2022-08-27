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

// remove matching v in [first, last) and return one past new end of range.
template <typename FwdIter, typename Value>
FwdIter
remove(FwdIter first, FwdIter last, const Value& v)
{
    // p1 will hold the first and all subsequent matching values.
    FwdIter p1 = std::find(first, last, v);
    if (p1 != last) {
        // Iterate p2 starting from p1 and swap all non-matching values.
        for (FwdIter p2 = p1; p2 != last; ++p2) {
                if (*p2 != v) {
                    std::iter_swap(p1, p2);
                    ++p1;
                }
        }
    }
    return p1;
}

}

TEST_CASE("[remove]")
{
    namespace alg = algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        int value;
        std::vector<int> expected;
        std::size_t expected_sz;
    };

    std::vector<test_case> test_cases{
        {
            "no matching values",
            {1, 2, 3, 4, 5}, 0,
            {1, 2, 3, 4, 5}, 5,
        },
        {
            "1 matching value -- first",
            {1, 2, 3, 4, 5}, 1,
            {2, 3, 4, 5, 1}, 4,
        },
        {
            "1 matching value -- middle",
            {1, 2, 3, 4, 5}, 3,
            {1, 2, 4, 5, 3}, 4,
        },
        {
            "1 matching value -- last",
            {1, 2, 3, 4, 5}, 5,
            {1, 2, 3, 4, 5}, 4,
        },
        {
            "2 matching value",
            {1, 2, 1, 4, 5}, 1,
            {2, 4, 5, 1, 1}, 3,
        },
        {
            "3 matching value",
            {1, 2, 1, 4, 1}, 1,
            {2, 4, 1, 1, 1}, 2,
        },
        {
            "4 matching value",
            {1, 1, 1, 4, 1}, 1,
            {4, 1, 1, 1, 1}, 1,
        },
        {
            "all matching value",
            {1, 1, 1, 1, 1}, 1,
            {1, 1, 1, 1, 1}, 0,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        auto end = alg::remove(std::begin(input_cp), std::end(input_cp), c.value);
        REQUIRE(std::distance(std::begin(input_cp), end) == c.expected_sz);
        REQUIRE(input_cp == c.expected);
    }
}
