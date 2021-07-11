#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// TreeNode is a node in a binary tree.
template <typename T>
struct TreeNode
{
    using TreeNodePtr = std::shared_ptr<TreeNode<T>>;

    TreeNode() = default;

    TreeNode(const T& data) : data(data) { }

    TreeNode<T>* insert_left(const T& v)
    {
        left = std::make_shared<TreeNode<T>>(v);
        return left.get();
    }

    TreeNode<T>* insert_right(const T& v)
    {
        right = std::make_shared<TreeNode<T>>(v);
        return right.get();
    }

    T data;
    TreeNodePtr left;
    TreeNodePtr right;
};

// make_tree returns tree initiailized from container of values.
template <typename T>
std::shared_ptr<TreeNode<T>>
make_tree(const std::vector<T>& values)
{
    std::shared_ptr<TreeNode<T>> root;

    using QueueEntry = TreeNode<T>*;
    std::queue<QueueEntry> nodes;

    for (const auto& v : values) {
        if (!root) {
            root = std::make_shared<TreeNode<T>>(v);
            nodes.emplace(root.get());
        }
        else {
            auto& node = nodes.front();
            if (!node->left) {
                nodes.emplace(node->insert_left(v));
            }
            else {
                nodes.emplace(node->insert_right(v));
                nodes.pop(); // Discard.
            }
        }
    }

    return root;
}

// make_vector_inorder returns vector initialized from inorder traversal.
template <typename T>
std::vector<T>
make_vector_inorder(const TreeNode<T>* root)
{
    std::vector<T> values;

    using VisitFunc = std::function<void(const TreeNode<T>*)>;

    VisitFunc visit_inorder = [&values, &visit_inorder](const TreeNode<T>* node) {
        if (node->left) {
            visit_inorder(node->left.get());
        }
        values.push_back(node->data);
        if (node->right) {
            visit_inorder(node->right.get());
        }
    };
    if (root) {
        visit_inorder(root);
    }

    return values;
}

// height returns the height of the tree.
template <typename T>
std::size_t
height(const TreeNode<T>* root)
{
    if (!root || (!root->left && !root->right)) {
        return 0; // Height of tree with 0 or 1 node is 0.
    }
    return 1 + std::max(height(root->left.get()), height(root->right.get()));
}

// size returns the number of nodes in the tree.
template <typename T>
std::size_t
size(const TreeNode<T>* root)
{
    if (!root) {
        return 0;
    }
    return 1 + size(root->left.get()) + size(root->right.get());
}

template <typename T>
struct LCAResult
{
    const TreeNode<T>* lca;
    int count;
};

// lca returns the least common ancestor of nodes in tree with values v1 and v2.
template <typename T>
LCAResult<T>
lca(const TreeNode<T>* root, const T& v1, const T& v2)
{
    if (!root) {
        return {nullptr, 0};
    }

    auto left_result = lca(root->left.get(), v1, v2);
    if (left_result.count == 2) {
        return left_result;
    }
    auto right_result = lca(root->right.get(), v1, v2);
    if (right_result.count == 2) {
        return right_result;
    }

    int sum = left_result.count + right_result.count;
    if (sum == 2) {
        return {root, sum}; // Current node is lca.
    }
    sum += (root->data == v1) + (root->data == v2);
    return {nullptr, sum};
}

}

TEST_CASE("[make_tree]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::vector<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced Tree.",
            {1, 2, 3, 4, 5, 6, 7},
            {4, 2, 5, 1, 6, 3, 7},
        },
        {
            "Unbalanced Tree.",
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
            {8, 4, 9, 2, 10, 5, 11, 1, 6, 3, 7},
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_tree(c.values);
        auto rcv = make_vector_inorder(root.get());
        REQUIRE(rcv == c.expected);
    }
}

TEST_CASE("[height]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::size_t expected;
    };

    std::vector<test_case> test_cases{
        {
            "height=0,size=0",
            {},
            0,
        },
        {
            "height=0,size=1",
            {1},
            0,
        },
        {
            "height=1,size=2",
            {1, 2},
            1,
        },
        {
            "height=1,size=3",
            {1, 2, 3},
            1,
        },
        {
            "height=2,size=4",
            {1, 2, 3, 4},
            2,
        },
        {
            "height=2,size=5",
            {1, 2, 3, 4, 5},
            2,
        },
        {
            "height=2,size=6",
            {1, 2, 3, 4, 5, 6},
            2,
        },
        {
            "height=2,size=7",
            {1, 2, 3, 4, 5, 6, 7},
            2,
        },
        {
            "height=3,size=8",
            {1, 2, 3, 4, 5, 6, 7, 8},
            3,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_tree(c.values);
        auto rcv = height(root.get());
        REQUIRE(rcv == c.expected);
    }
}

TEST_CASE("[size]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::size_t expected;
    };

    std::vector<test_case> test_cases{
        {
            "height=0,size=0",
            {},
            0,
        },
        {
            "height=0,size=1",
            {1},
            1,
        },
        {
            "height=1,size=2",
            {1, 2},
            2,
        },
        {
            "height=1,size=3",
            {1, 2, 3},
            3,
        },
        {
            "height=2,size=4",
            {1, 2, 3, 4},
            4,
        },
        {
            "height=2,size=5",
            {1, 2, 3, 4, 5},
            5,
        },
        {
            "height=2,size=6",
            {1, 2, 3, 4, 5, 6},
            6,
        },
        {
            "height=2,size=7",
            {1, 2, 3, 4, 5, 6, 7},
            7,
        },
        {
            "height=3,size=8",
            {1, 2, 3, 4, 5, 6, 7, 8},
            8,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_tree(c.values);
        auto rcv = size(root.get());
        REQUIRE(rcv == c.expected);
    }
}

TEST_CASE("[lca]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        int v1;
        int v2;
        std::optional<int> expected;
    };

    std::vector<test_case> test_cases{
        {
            "LCA is parent of siblings",
            {1, 2, 3, 4, 5, 6, 7, 8},
            4, 5,
            2,
        },
        {
            "LCA with children at different levels.",
            {1, 2, 3, 4, 5, 6, 7, 8},
            5, 8,
            2,
        },
        {
            "LCA is root and children in different subtrees.",
            {1, 2, 3, 4, 5, 6, 7, 8},
            7, 8,
            1,
        },
        {
            "LCA is root and children at same levels.",
            {1, 2, 3, 4, 5, 6, 7, 8},
            2, 3,
            1,
        },
        {
            "LCA not found.",
            {1, 2, 3, 4, 5, 6, 7, 8},
            99, 5,
            std::nullopt,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_tree(c.values);
        auto rcv = lca(root.get(), c.v1, c.v2);
        if (c.expected == std::nullopt) {
            REQUIRE(rcv.lca == nullptr);
        }
        else {
            REQUIRE(rcv.lca->data == *c.expected);
        }
    }
}
