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
combos(const std::vector<T>& elems, std::size_t k,
       std::vector<std::vector<T>>& all_combos,
       std::vector<T>& curr_combo,
       std::size_t start_index=0)
{
    if (curr_combo.size() == k) {
        // Base case of recursion is reached when current
        // combination contains k objects.
        all_combos.push_back(curr_combo);
        return;
    }

    for (std::size_t i = start_index; i < elems.size(); ++i) {
        // Add element to current combination.
        curr_combo.push_back(elems[i]);
        // Recursively compute combinations using remaining elements.
        combos(elems, k, all_combos, curr_combo, i + 1);
        // Remove element from current combination.
        curr_combo.pop_back();
    }
}

// combos returns all combinations of n objects choosing k e.g. C(n,k).
template <typename T>
std::vector<std::vector<T>>
combos(const std::vector<T>& elems, std::size_t k)
{
    std::vector<std::vector<T>> all_combos;
    std::vector<T> curr_combo;
    combos(elems, k, all_combos, curr_combo);

    return all_combos;
}

}

TEST_CASE("[combos]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> elems;
        std::size_t k;
        std::vector<std::vector<int>> expected;
    };

    std::vector<test_case> test_cases{
        {
            "C(1,1)",
            {1}, 1,
            { {1}, },
        },
        {
            "C(2,1)",
            {1, 2}, 1,
            { {1}, {2}, },
        },
        {
            "C(2,2)",
            {1, 2}, 2,
            { {1, 2} },
        },
        {
            "C(3,1)",
            {1, 2, 3}, 1,
            { {1}, {2}, {3} },
        },
        {
            "C(3,2)",
            {1, 2, 3}, 2,
            { {1, 2}, {1, 3}, {2, 3} },
        },
        {
            "C(3,3)",
            {1, 2, 3}, 3,
            { {1, 2, 3} },
        },
        {
            "C(4,1)",
            {1, 2, 3, 4}, 1,
            { {1}, {2}, {3}, {4} },
        },
        {
            "C(4,2)",
            {1, 2, 3, 4}, 2,
            { {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} },
        },
        {
            "C(4,3)",
            {1, 2, 3, 4}, 3,
            { {1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4} },
        },
        {
            "C(4,4)",
            {1, 2, 3, 4}, 4,
            { {1, 2, 3, 4} },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = combos(c.elems, c.k);
        REQUIRE(rcv == c.expected);
    }
}
