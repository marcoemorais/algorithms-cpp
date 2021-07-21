#include <array>
#include <cstddef>
#include <iterator>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// prefix_sum_1d returns the 1d prefix sum.
template <typename Cont>
std::vector<typename Cont::value_type>
prefix_sum_1d(const Cont& elems)
{
    using T = typename Cont::value_type;

    // Initialize container by copying the input.
    std::vector<T> prefix_sum(std::begin(elems), std::end(elems));

    for (std::size_t i = 1; i < elems.size(); ++i) {
        prefix_sum[i] += prefix_sum[i-1];
    }

    return prefix_sum;
}

// Vector is an alias for a column vector.
template <typename T> using Vector = std::vector<T>;

// Matrix2d is an alias for a matrix of column vectors.
template <typename T> using Matrix2d = std::vector<Vector<T>>;

// prefix_sum_2d returns the 2d prefix sum.
template <typename T>
Matrix2d<T>
prefix_sum_2d(const Matrix2d<T>& m)
{
    std::size_t nrow = m.size();
    std::size_t ncol = nrow > 0 ? m[0].size() : 0;

    // Initialize matrix by copying the input.
    Matrix2d<T> prefix_sum = m;

    // Compute the prefix sum along the columns.
    for (std::size_t i = 0; i < nrow; ++i) {
        for (std::size_t j = 1; j < ncol; ++j) {
            prefix_sum[i][j] += prefix_sum[i][j-1];
        }
    }

    // Compute the prefix sum along the rows.
    for (std::size_t j = 0; j < ncol; ++j) {
        for (std::size_t i = 1; i < nrow; ++i) {
            prefix_sum[i][j] += prefix_sum[i-1][j];
        }
    }

    return prefix_sum;
}

}

TEST_CASE("[prefix_sum_1d:vector]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> elems;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Empty input.",
            {},
            {},
        },
        {
            "size=1",
            {1},
            {1},
        },
        {
            "size=2",
            {1, 2},
            {1, 3},
        },
        {
            "size=3",
            {1, 2, 3},
            {1, 3, 6},
        },
        {
            "size=4",
            {1, 2, 3, 4},
            {1, 3, 6, 10},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = prefix_sum_1d(c.elems);
        REQUIRE(rcv == c.expected);
    }
}

TEST_CASE("[prefix_sum_1d:array]")
{
    using namespace algorithms;

    {
        INFO("Empty input.");
        std::array<int, 0> elems{};
        std::vector<int> expected{};
        auto rcv = prefix_sum_1d(elems);
        REQUIRE(rcv == expected);
    }

    {
        INFO("size=1");
        std::array<int, 1> elems{1};
        std::vector<int> expected{1};
        auto rcv = prefix_sum_1d(elems);
        REQUIRE(rcv == expected);
    }

    {
        INFO("size=2");
        std::array<int, 2> elems{1, 2};
        std::vector<int> expected{1, 3};
        auto rcv = prefix_sum_1d(elems);
        REQUIRE(rcv == expected);
    }

    {
        INFO("size=3");
        std::array<int, 3> elems{1, 2, 3};
        std::vector<int> expected{1, 3, 6};
        auto rcv = prefix_sum_1d(elems);
        REQUIRE(rcv == expected);
    }

    {
        INFO("size=4");
        std::array<int, 4> elems{1, 2, 3, 4};
        std::vector<int> expected{1, 3, 6, 10};
        auto rcv = prefix_sum_1d(elems);
        REQUIRE(rcv == expected);
    }
}

TEST_CASE("[prefix_sum_2d]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        Matrix2d<int> input;
        Matrix2d<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Empty input.",
            {},
            {},
        },
        {
            "2x2",
            {
                { 1, 2},
                { 4, 5},
            },
            {
                { 1,  3},
                { 5, 12},
            },
        },
        {
            "3x3",
            {
                { 1, 2, 3},
                { 4, 5, 6},
                { 7, 8, 9},
            },
            {
                { 1,  3,  6},
                { 5, 12, 21},
                {12, 27, 45},
            },
        },
        {
            "4x4",
            {
                { 1,  2,  3,  4},
                { 5,  6,  7,  8},
                { 9, 10, 11, 12},
                {13, 14, 15, 16},
            },
            {
                { 1,  3,  6, 10},
                { 6, 14, 24, 36},
                {15, 33, 54, 78},
                {28, 60, 96,136},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto rcv = prefix_sum_2d(c.input);
        REQUIRE(rcv == c.expected);
    }
}
