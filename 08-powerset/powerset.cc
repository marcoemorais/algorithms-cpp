#include <cstddef>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

template <typename T>
void
powerset(const std::vector<T>& elems, std::vector<std::vector<T>>& all_sets,
         std::size_t k, std::vector<T>& curr_subset)
{
    if (k == elems.size()) {
        // Base case of recursion is reached when k is equal
        // the index of the last element of the input.
        all_sets.push_back(curr_subset);
        return;
    }

    // Recursively obtain all subsets with k-th element.
    curr_subset.push_back(elems[k]);
    powerset(elems, all_sets, k+1, curr_subset);
    // Recursively obtain all subsets without k-th element.
    curr_subset.pop_back();
    powerset(elems, all_sets, k+1, curr_subset);
}

// powerset returns all possible subsets of the input.
template <typename T>
std::vector<std::vector<T>>
powerset(const std::vector<T>& elems)
{
    std::vector<std::vector<T>> all_sets;
    std::size_t k = 0;
    std::vector<T> curr_subset;
    powerset(elems, all_sets, k, curr_subset);

    return all_sets;
}

}

TEST_CASE("[powerset]")
{
    using namespace algorithms;

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
            { {1}, {}, },
        },
        {
            "2-element set.",
            {1, 2},
            { {1, 2}, {1}, {2}, {}, },
        },
        {
            "3-element set.",
            {1, 2, 3},
            { {1, 2, 3}, {1, 2}, {1, 3}, {1}, {2, 3}, {2}, {3}, {}, },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = powerset(c.elems);
        REQUIRE(rcv == c.expected);
    }
}
