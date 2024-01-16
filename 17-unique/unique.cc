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

// unique removes adjacent duplicates from sorted sequence [first, last).
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
                *p1 = *p2; // Overwrite duplicate p1 with p2.
            }
        }
        ++p2;
    }
    ++p1; // One past last unique element.

    return p1;
}

// unique removes count adjacent duplicates from sorted sequence [first, last).
template <typename FwdIter>
FwdIter
unique_count(FwdIter first, FwdIter last, size_t count=1) // count=1, no dupes
{
    size_t n = std::distance(first, last);
    if (n <= count) {
        return last;
    }

    FwdIter read = first + count;
    FwdIter write = read;
    while (read != last) {
        *write = *read;
        if (*write != *(write - count)) {
            ++write; // Stop advancing write after count values.
        }
        ++read;
    }
    return write;
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
    };

    std::vector<test_case> test_cases{
        {
            "no duplicate values",
            {1, 2, 3, 4, 5},
            {1, 2, 3, 4, 5},
        },
        {
            "1 duplicate pair -- first",
            {1, 1, 2, 3, 4},
            {1, 2, 3, 4},
        },
        {
            "1 duplicate pair -- middle",
            {1, 2, 3, 3, 4},
            {1, 2, 3, 4},
        },
        {
            "1 duplicate pair -- last",
            {1, 2, 3, 4, 4},
            {1, 2, 3, 4},
        },
        {
            "2 duplicate pair",
            {1, 1, 2, 3, 3},
            {1, 2, 3},
        },
        {
            "3 duplicate values",
            {1, 1, 1, 2, 3},
            {1, 2, 3},
        },
        {
            "4 duplicate values",
            {1, 1, 1, 1, 2},
            {1, 2},
        },
        {
            "all duplicate values",
            {1, 1, 1, 1, 1},
            {1},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        auto new_end = alg::unique(std::begin(input_cp),
                                   std::end(input_cp));
        REQUIRE(std::distance(std::begin(input_cp), new_end) ==
                std::size(c.expected));
        input_cp.erase(new_end, end(input_cp)); // Ignore trailing dupes.
        REQUIRE(input_cp == c.expected);
    }
}

TEST_CASE("[unique_count, count=1]")
{
    namespace alg = algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "no duplicate values",
            {1, 2, 3, 4, 5},
            {1, 2, 3, 4, 5},
        },
        {
            "1 duplicate pair -- first",
            {1, 1, 2, 3, 4},
            {1, 2, 3, 4},
        },
        {
            "1 duplicate pair -- middle",
            {1, 2, 3, 3, 4},
            {1, 2, 3, 4},
        },
        {
            "1 duplicate pair -- last",
            {1, 2, 3, 4, 4},
            {1, 2, 3, 4},
        },
        {
            "2 duplicate pair",
            {1, 1, 2, 3, 3},
            {1, 2, 3},
        },
        {
            "3 duplicate values",
            {1, 1, 1, 2, 3},
            {1, 2, 3},
        },
        {
            "4 duplicate values",
            {1, 1, 1, 1, 2},
            {1, 2},
        },
        {
            "all duplicate values",
            {1, 1, 1, 1, 1},
            {1},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        auto new_end = alg::unique_count(std::begin(input_cp),
                                         std::end(input_cp));
        REQUIRE(std::distance(std::begin(input_cp), new_end) ==
                std::size(c.expected));
        input_cp.erase(new_end, end(input_cp)); // Ignore trailing dupes.
        REQUIRE(input_cp == c.expected);
    }
}

TEST_CASE("[unique_count, count=n]")
{
    namespace alg = algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        size_t count;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "no duplicate values",
            {1, 2, 3, 4, 5, 6, 7, 8},
            2,
            {1, 2, 3, 4, 5, 6, 7, 8},
        },
        {
            "triple -- first",
            {1, 1, 1, 2, 3, 4, 5, 6},
            2,
            {1, 1, 2, 3, 4, 5, 6},
        },
        {
            "triple -- middle",
            {1, 2, 2, 2, 3, 4, 5, 6},
            2,
            {1, 2, 2, 3, 4, 5, 6},
        },
        {
            "triple -- last",
            {1, 2, 3, 3, 3},
            2,
            {1, 2, 3, 3},
        },
        {
            "2 triples",
            {1, 1, 1, 2, 2, 2},
            2,
            {1, 1, 2, 2},
        },
        {
            "3 triples",
            {1, 1, 1, 2, 2, 2, 3, 3, 3},
            2,
            {1, 1, 2, 2, 3, 3},
        },
        {
            "all duplicate values, count = 1",
            {1, 1, 1, 1, 1},
            1,
            {1},
        },
        {
            "all duplicate values, count = 2",
            {1, 1, 1, 1, 1},
            2,
            {1, 1},
        },
        {
            "all duplicate values, count = 3",
            {1, 1, 1, 1, 1},
            3,
            {1, 1, 1},
        },
        {
            "all duplicate values, count = 4",
            {1, 1, 1, 1, 1},
            4,
            {1, 1, 1, 1},
        },
        {
            "all duplicate values, count = 5",
            {1, 1, 1, 1, 1},
            5,
            {1, 1, 1, 1, 1},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        auto new_end = alg::unique_count(std::begin(input_cp),
                                         std::end(input_cp),
                                         c.count);
        REQUIRE(std::distance(std::begin(input_cp), new_end) ==
                std::size(c.expected));
        input_cp.erase(new_end, end(input_cp)); // Ignore trailing dupes.
        REQUIRE(input_cp == c.expected);
    }
}
