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

// rotate elements from [first, last) so that middle appears first.
template <typename FwdIter>
void
rotate(FwdIter first, FwdIter middle, FwdIter last)
{
    if (first == middle || middle == last) {
        return;
    }

    FwdIter write = first;
    FwdIter read = middle;
    FwdIter next_middle = first;

    while (read != last) {
        if (write == next_middle) {
            next_middle = read;
        }
        std::iter_swap(read, write);
        ++read;
        ++write;
    }

    algorithms::rotate(write, next_middle, last); // rotate subsequence.
}

}

TEST_CASE("[unique]")
{
    namespace alg = algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::size_t first;
        std::size_t middle;
        std::size_t last;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "rotate first + 1 to first",
            {1, 2, 3, 4, 5},
            0, 1, 5,
            {2, 3, 4, 5, 1},
        },
        {
            "rotate first + 2 to first",
            {1, 2, 3, 4, 5},
            0, 2, 5,
            {3, 4, 5, 1, 2},
        },
        {
            "rotate first + 3 to first",
            {1, 2, 3, 4, 5},
            0, 3, 5,
            {4, 5, 1, 2, 3},
        },
        {
            "rotate first + 4 to first",
            {1, 2, 3, 4, 5},
            0, 4, 5,
            {5, 1, 2, 3, 4},
        },
        {
            "no rotation",
            {1, 2, 3, 4, 5},
            0, 0, 5,
            {1, 2, 3, 4, 5},
        },
        {
            "first rotation",
            {1, 2, 3, 4, 5},
            0, 1, 3,
            {2, 3, 1, 4, 5},
        },
        {
            "middle rotation",
            {1, 2, 3, 4, 5},
            1, 2, 4,
            {1, 3, 4, 2, 5},
        },
        {
            "end rotation",
            {1, 2, 3, 4, 5},
            2, 3, 5,
            {1, 2, 4, 5, 3},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto input_cp = c.input;
        alg::rotate(std::begin(input_cp) + c.first,
                    std::begin(input_cp) + c.middle,
                    std::begin(input_cp) + c.last);
        REQUIRE(input_cp == c.expected);
    }
}
