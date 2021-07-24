#include <cstddef>
#include <cmath>
#include <random>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// monte_carlo_pi returns an estimate of pi.
double monte_carlo_pi(std::size_t niter=10'000'000)
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    std::uniform_real_distribution<double> dis(0, 1); // Open interval: [0,1)

    // Count the number of points that fall within the quarter
    // circle circumscribed by a square with sides of length 1.
    std::size_t count = 0;
    for (std::size_t i = 0; i < niter; ++i) {
        double x = dis(gen), y = dis(gen);
        if (x*x + y*y < 1) {
            ++count;
        }
    }

    return 4.*double(count)/niter;
}

}

TEST_CASE("[monte_carlo_pi]")
{
    using namespace algorithms;

    struct test_case
    {
        std::size_t niter;
        double epsilon;
    };

    std::vector<test_case> test_cases{
        {
            10'000,
            0.1,      // 1 digit accuracy, 3
        },
        {
            100'000,
            0.01,     // 2 digit accuracy, 3.1
        },
        {
            1'000'000,
            0.001,    // 3 digit accuracy, 3.14
        },
        {
            10'000'000,
            0.001,    // 3 digit accuracy, 3.14
        },
    };

    for (const auto& c :test_cases) {
        INFO(c.niter);
        auto rcv = monte_carlo_pi(c.niter);
        REQUIRE(rcv == doctest::Approx(M_PI).epsilon(c.epsilon));
    }
}
