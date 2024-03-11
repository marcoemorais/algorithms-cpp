#include <cassert>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// binomial returns the binomial coefficents of n using dynamic programming.
std::vector<int>
binomial(int n)
{
    assert(n >= 0);
    std::vector<int> coeffs(n + 1), prev(n + 1);

    for (int i = 0; i <= n; ++i) {
        prev = coeffs;
        coeffs[0] = 1, coeffs[i] = 1; // First and last always 1.
        for (int j = 1; j < i; ++j) {
            coeffs[j] = prev[j - 1] + prev[j];
        }
    }

    return coeffs;
}

}

TEST_CASE("[binomial]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        int n;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "n=0",
            0,
            {1},
        },
        {
            "n=1",
            1,
            {1, 1},
        },
        {
            "n=2",
            2,
            {1, 2, 1},
        },
        {
            "n=3",
            3,
            {1, 3, 3, 1},
        },
        {
            "n=4",
            4,
            {1, 4, 6, 4, 1},
        },
        {
            "n=5",
            5,
            {1, 5, 10, 10, 5, 1},
        },
        {
            "n=6",
            6,
            {1, 6, 15, 20, 15, 6, 1},
        },
        {
            "n=7",
            7,
            {1, 7, 21, 35, 35, 21, 7, 1},
        },
        {
            "n=8",
            8,
            {1, 8, 28, 56, 70, 56, 28, 8, 1},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = binomial(c.n);
        REQUIRE(rcv == c.expected);
        // Verify sum(coef) = 2^n.
        int sum = std::accumulate(begin(rcv), end(rcv), 0);
        int pow2n = std::pow(2, c.n);
        REQUIRE(sum == pow2n);
    }
}
