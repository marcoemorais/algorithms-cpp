#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// coin_change returns the minimum number of coins to sum to S.
std::vector<int>
coin_change(int S, const std::unordered_set<int>& currency,
            std::unordered_map<int, std::vector<int>> cache)
{
    std::vector<int> minsoltn; // Minimum number of coins.

    for (const auto& coin : currency) {
        if (coin == S) {
            minsoltn = {coin};
            break; // No solution with fewer coins is possible.
        }
        else if (coin < S) {
            // Compose a solution for the subproblem of S - coin.
            std::vector<int> anysoltn;
            int remaining = S - coin;
            if (cache.count(remaining)) {
                anysoltn = cache[remaining];
            }
            else {
                anysoltn = coin_change(remaining, currency, cache);
            }
            if (!anysoltn.empty()) {
                // Add coin to candidate solution and commpare to minimum.
                anysoltn.push_back(coin);
                if (minsoltn.empty() || anysoltn.size() < minsoltn.size()) {
                    minsoltn = anysoltn;
                }
            }
        }
    }

    cache[S] = minsoltn;
    return minsoltn;
}

}

TEST_CASE("[coin_change]")
{
    using namespace algorithms;

    struct test_case
    {
        int S;
        std::unordered_set<int> currency;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            14,
            {1, 3, 7, 8},
            {7, 7},
        },
        {
            12,
            {1, 2, 5},
            {2, 5, 5},
        },
        {
            10,
            {1, 3, 4},
            {3, 3, 4},
        },
        {
            6,
            {1, 3, 4},
            {3, 3},
        },
        {
            3,
            {1, 4},
            {1, 1, 1},
        },
        // Edge case, currency too large.
        {
            1,
            {5},
            {},
        },
        // Edge case, no possible sum.
        {
            7,
            {2, 4},
            {},
        },
    };

    for (const auto& c : test_cases) {
        std::unordered_map<int, std::vector<int>> cache;
        auto rcv = coin_change(c.S, c.currency, cache);
        std::sort(rcv.begin(), rcv.end());
        REQUIRE(rcv == c.expected);
    }
}
