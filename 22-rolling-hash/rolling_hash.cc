#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

using DefaultHashT = std::uint64_t;

constexpr uint64_t DEFAULT_BASE{131}; // Next prime after 2^7.

// pow_base returns base^size for the hash function.
template <typename HashT = DefaultHashT,
          HashT base = DEFAULT_BASE>
HashT pow_base_size(std::size_t size)
{
    return std::pow(base, size - 1);
}

// rolling_hash returns the hash of a rolling window in O(1).
template <typename FwdIter,
          typename HashT = DefaultHashT,
          HashT base = DEFAULT_BASE>
HashT
rolling_hash(FwdIter left, FwdIter right,
             const HashT& pow_base_size,
             const HashT& prev_hash)
{
    // Compute a new hash from the previous by subtracting
    // the value `left` from the previous hash using Horner's
    // Rule and adding the value `right` to the new hash.
    return (prev_hash - pow_base_size * *left) * base + *right;
}

// window_hash returns the hash of a window from [first, last) in O(n).
template <typename FwdIter,
          typename HashT = DefaultHashT,
          HashT base = DEFAULT_BASE>
HashT window_hash(FwdIter first, FwdIter last)
{
    HashT new_hash{0};
    // Compute a new hash by traversing [first, last).
    while (first != last) {
        new_hash = (new_hash * base) + *first++;
    }
    return new_hash;
}

}

TEST_CASE("[rolling_hash]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string input;
    };

    std::vector<test_case> test_cases{
        { "hello" },
        { "world" },
        { "rolling" },
        { "hash" },
    };

    for (const auto& c : test_cases) {
        // Compute the full hash of the input.
        auto h1 = window_hash(std::begin(c.input), std::end(c.input));

        // Duplicate the result by stepping through the
        // input char-by-char computing the rolling hash
        // and comparing the result to the full hash after
        // stepping through all size(input) characters.
        auto h2 = h1;
        auto padded_input = c.input + c.input;
        auto window_size = pow_base_size(size(c.input));
        for (size_t i = 0; i < size(padded_input) - size(c.input); ++i) {
            h2 = rolling_hash(&padded_input[i],
                              &padded_input[i + size(c.input)],
                              window_size,
                              h2);
        }

        // Compare h1 and h2.
        REQUIRE(h1 == h2);
    }
}
