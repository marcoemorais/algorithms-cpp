#include <cstddef>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

template <typename T>
void
perms(const std::vector<T>& elems, std::vector<std::vector<T>>& all_perms,
      std::vector<bool>& pivots, std::vector<T>& curr_perm)
{
    if (curr_perm.size() == elems.size()) {
        // Base case of recursion is reached when current
        // permutation contains all elements of original set.
        all_perms.push_back(curr_perm);
        return;
    }

    for (std::size_t i = 0; i < elems.size(); ++i) {
        if (pivots[i]) {
            continue; // Skip.
        }
        // Add element to current permutation and set as pivot.
        curr_perm.push_back(elems[i]);
        pivots[i] = true;
        // Recursively compute permutations excluding pivots.
        perms(elems, all_perms, pivots, curr_perm);
        // Remove element from current permutation and clear pivot.
        curr_perm.pop_back();
        pivots[i] = false;
    }
}


// perms returns the set of all permutations of the input.
template <typename T>
std::vector<std::vector<T>>
perms(const std::vector<T>& elems)
{
    std::vector<std::vector<T>> all_perms;
    std::vector<bool> pivots(elems.size());
    std::vector<T> curr_perm;
    perms(elems, all_perms, pivots, curr_perm);

    return all_perms;
}

TEST_CASE("[perms]")
{
    struct test_case
    {
        std::string name;
        std::vector<int> elems;
        std::vector<std::vector<int>> expected;
    };

    std::vector<test_case> test_cases{
        {
            "1-element set.",
            {1},
            { {1} },
        },
        {
            "2-element set.",
            {1, 2},
            { {1, 2}, {2, 1}, },
        },
        {
            "3-element set.",
            {1, 2, 3},
            { {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1} },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = perms(c.elems);
        REQUIRE(rcv == c.expected);
    }
}
