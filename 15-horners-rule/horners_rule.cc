#include <cstddef>
#include <stdexcept>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// Polynomial is an array of coefficients $a_N, ..., a_1, a_0$ of decreasing degree.
template <typename T>
using Polynomial = std::vector<T>;

// Evaluate the polynomial at x using Horner's Rule.
// $p(x) = a_0 + a_1 x + a_2 x^2 + ... + a_N x^N$
template <typename T>
T evaluate(const Polynomial<T>& coefs, const T& x)
{
    if (coefs.empty()) {
        throw std::domain_error("cannot evaluate polynomial with empty coefficients");
    }

    T result = coefs.front();
    for (std::size_t i = 1; i < coefs.size(); ++i) {
        result = coefs[i] + x * result;
    }

    return result;
}

}

TEST_CASE("[horners_rule]")
{
    using namespace algorithms;

    struct test_case
    {
        Polynomial<double> p;
        double x;
        double expected;
    };

    std::vector<test_case> test_cases{
        // Quadratic polynomials.
        {
            {1,2,3}, 1., 6.
        },
        {
            {1,2,3}, 2., 11.
        },
        {
            {1,2,3}, 3., 18.
        },
        // Cubic polynomials.
        {
            {4,3,2,1}, 1., 10.
        },
        {
            {4,3,2,1}, 2., 49.
        },
        {
            {4,3,2,1}, 3., 142.
        },
        // Hermite polynomials.
        {
            {1.,0.,-21.,0.,105.,0.,-105.,0.}, 1., -20.
        },
        {
            {1.,0.,-21.,0.,105.,0.,-105.,0.}, 2., 86.
        },
        {
            {1.,0.,-21.,0.,105.,0.,-105.,0.}, 3., -396.
        }
    };

    for (const auto& c : test_cases) {
        auto rcv = evaluate(c.p, c.x);
        REQUIRE(rcv == doctest::Approx(c.expected));
    }

    {
        // Empty coefficients, domain error.
        Polynomial<double> p{};
        REQUIRE_THROWS_AS(evaluate(p, 0.), std::domain_error);
    }
}
