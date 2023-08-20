#include <cstddef>
#include <cmath>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace algorithms
{

// is_valid_queen_position returns true when no queen can attack position.
bool
is_valid_queen_position(const std::vector<int>& queens, int position)
{
    // Queens is vector with the position of each queen in the row.
    for (std::size_t i = 0; i < queens.size(); ++i) {
        // If column == 0, then a queen attacks along column.
        int column = std::abs(queens[i] - position);
        if (column == 0) {
            return false;
        }
        // If diagonal == column, then a queen attacks along diagonal.
        int diagonal = queens.size() - i;
        if (diagonal == column) {
            return false;
        }
    }
    return true;
}

void
nqueens(int n, std::vector<int>& queens,
        std::vector<std::vector<int>>& all_queens)
{
    if (queens.size() == std::size_t(n)) {
        // Base case is reached with n mutually non-attacking queens.
        all_queens.push_back(queens);
        return;
    }
    // Check whether a queen can be placed at any position on current row.
    for (int position = 0; position < n; ++position) {
        if (is_valid_queen_position(queens, position)) {
            queens.push_back(position);
            nqueens(n, queens, all_queens); // Advance to next row.
            queens.pop_back(); // Backtrack.
        }
    }
}

// nqueens returns all unique arrangements of queens on n X n board.
// An arrangement is valid when all n queens are mutually non-attacking.
std::vector<std::vector<int>>
nqueens(int n)
{
    // queens is a vector with the posiiton of each queen in the row.
    std::vector<int> queens;
    std::vector<std::vector<int>> solution;
    nqueens(n, queens, solution);

    return solution;
}

}

TEST_CASE("[nqueens]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        int n;
        std::vector<std::vector<int>> expected;
    };

    std::vector<test_case> test_cases{
        {
            "2x2 board",
            2,
            {},
        },
        {
            "3x3 board",
            3,
            {},
        },
        {
            "4x4 board",
            4,
            {
                {1, 3, 0, 2},
                {2, 0, 3, 1}
            },
        },
        {
            "5x5 board",
            5,
            {
                {0, 2, 4, 1, 3}, {0, 3, 1, 4, 2},
                {1, 3, 0, 2, 4}, {1, 4, 2, 0, 3},
                {2, 0, 3, 1, 4}, {2, 4, 1, 3, 0},
                {3, 0, 2, 4, 1}, {3, 1, 4, 2, 0},
                {4, 1, 3, 0, 2}, {4, 2, 0, 3, 1},
            },
        },

    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = nqueens(c.n);
        REQUIRE(rcv == c.expected);
    }
}
