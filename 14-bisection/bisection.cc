#include <cassert>
#include <cmath>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// bisection returns the root of a scalar valued function over bracket [a,b].
template <typename Func>
double bisection(Func fx, double a, double b, double tol=1e-6)
{
    assert(a < b);
    while (fabs(a-b) > tol) {
        double m = a + (b - a) / 2.;
        auto signfxa = fx(a) > 0 ? 1 : 0;
        auto signfxm = fx(m) > 0 ? 1 : 0;
        if (signfxa == signfxm) {
            // a and m are on the same side of the axis.
            a = m;
        }
        else {
            // a and m are on opposite side of axis.
            b = m;
        }
    }
    return a;
}

struct CubeRoot
{
    explicit CubeRoot(double x3) : x3(x3) { }

    double operator()(double x) const
    {
        // Find root of f(x) - x^3 = 0.
        return x*x*x - x3;
    }

    double x3;
};

}

TEST_CASE("[bisection]")
{
    using namespace algorithms;

    constexpr double tol = 1e-6;
    constexpr double eps = 1e-5;

    struct test_case
    {
        double x3;
        double expected;
    };

    std::vector<test_case> test_cases{
        // Use bisection to find the cube root of x.
        {
            8.,
            2.,
        },
        {
            9.,
            2.080083,
        },
        {
            12.,
            2.289428,
        },
        {
            15.,
            2.466212,
        },
        {
            18.,
            2.620741,
        },
        {
            27.,
            3.,
        },
    };

    for (const auto& c : test_cases) {
        CubeRoot fx(c.x3);
        double a = 0., b = c.x3;
        auto rcv = bisection(fx, a, b, tol);
        REQUIRE(rcv == doctest::Approx(c.expected).epsilon(eps));
    }
}
