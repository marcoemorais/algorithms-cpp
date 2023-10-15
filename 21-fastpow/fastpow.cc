#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// fastpow uses divide-and-conquer to compute x^y.
double fastpow(double x, long y)
{
    if (y == 0) {
        return 1.;
    }
    if (y < 0.) {
        y = -y;
        x = 1./x;
    }
    double result = 1.;
    while (y) {
        // Compute x^y as the product of x^{y/2} * x^{y/2}.
        if (y & 1) {
            result *= x; // Odd exponent requires additional multiple.
        }
        x = x * x;  // Square x.
        y = y >> 1; // Divide exponent by 2.
    }
    return result;
}

}

TEST_CASE("[fastpow]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        double x;
        long y;
        double expected;
    };

    std::vector<test_case> test_cases{
        // Powers of 2.
        {
            "pow(2,-4) = 1./16",
            2., -4,
            1./16
        },
        {
            "pow(2,-3) = 1./8",
            2., -3,
            1./8
        },
        {
            "pow(2,-2) = 1./4",
            2., -2,
            1./4
        },
        {
            "pow(2,-1) = 1./2",
            2., -1,
            1./2
        },
        {
            "pow(2,0) = 1.",
            2., 0,
            1.
        },
        {
            "pow(2,1) = 2.",
            2., 1,
            2.
        },
        {
            "pow(2,2) = 4.",
            2., 2,
            4.
        },
        {
            "pow(2,3) = 8.",
            2., 3,
            8.
        },
        {
            "pow(2,4) = 16.",
            2., 4,
            16.
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = fastpow(c.x, c.y);
        REQUIRE(rcv == c.expected);
    }
}
