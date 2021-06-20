#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// bsearch returns element matching value.
template <typename RandomIt,
          typename T = typename std::iterator_traits<RandomIt>::value_type>
std::optional<T>
bsearch(RandomIt first, RandomIt last, const T& v)
{
    auto len = std::distance(first, last);
    if (len < 1) {
        return std::nullopt; // No match.
    }

    auto mid = first + len/2;
    if (v < *mid) {
        return bsearch(first, mid, v);
    }
    else if (v > *mid) {
        return bsearch(mid+1, last, v);
    }
    return *mid; // Exact match.
}

}

TEST_CASE("[bsearch]")
{
    using namespace algorithms;

    // BinarySearchResult holds result of binary search.
    using BinarySearchResult = std::pair<int, std::optional<int>>;

    struct test_case
    {
        std::string name;
        std::vector<int> input;
        std::vector<BinarySearchResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Empty input.",
            {},
            {
                {0, std::nullopt},
            },
        },
        {
            "1 element input.",
            {1},
            {
                {0, std::nullopt},
                {1, 1},
                {2, std::nullopt},
            },
        },
        {
            "2 element input.",
            {1, 2},
            {
                {0, std::nullopt},
                {1, 1},
                {2, 2},
                {3, std::nullopt},
            },
        },
        {
            "Sequential input.",
            {1, 2, 3, 4, 5},
            {
                {0, std::nullopt},
                {1, 1},
                {2, 2},
                {3, 3},
                {4, 4},
                {5, 5},
                {6, std::nullopt},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        for (const auto& expect : c.expected) {
            INFO(expect.first);
            auto rcv = bsearch(std::begin(c.input), std::end(c.input),
                               expect.first);
            if (expect.second == std::nullopt) {
                REQUIRE(rcv == std::nullopt);
            }
            else {
                REQUIRE(rcv != std::nullopt);
                REQUIRE(*rcv == *(expect.second));
            }
        }
    }
}
