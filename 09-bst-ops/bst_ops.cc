#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

// BSTNode is a node in a binary search tree.
template <typename T>
struct BSTNode
{
    using BSTNodePtr = std::shared_ptr<BSTNode<T>>;

    BSTNode() = default;

    BSTNode(const T& data) : data(data) { }

    BSTNode<T>* insert(const T& v)
    {
        if (v < data) {
            if (left) {
                return left->insert(v);
            }
            left = std::make_shared<BSTNode<T>>(v);
            return left.get();
        }
        else if (v > data) {
            if (right) {
                return right->insert(v);
            }
            right = std::make_shared<BSTNode<T>>(v);
            return right.get();
        }
        return this; // Ignore duplicates.
    }

    T data;
    BSTNodePtr left;
    BSTNodePtr right;
};

// make_bst returns bst initialized from container of values.
template <typename T>
std::shared_ptr<BSTNode<T>>
make_bst(const std::vector<T>& values)
{
    std::shared_ptr<BSTNode<T>> root;

    for (const auto& v : values) {
        if (!root) {
            root = std::make_shared<BSTNode<T>>(v);
        }
        else {
            root->insert(v);
        }
    }

    return root;
}

// make_vector_inorder returns vector initialized from inorder traversal.
template <typename T>
std::vector<T>
make_vector(const BSTNode<T>* root)
{
    std::vector<T> values;

    using VisitFunc = std::function<void(const BSTNode<T>*)>;

    VisitFunc visit_inorder = [&values, &visit_inorder](const BSTNode<T>* node) {
        if (node->left) {
            visit_inorder(node->left.get());
        }
        values.push_back(node.data);
        if (node->right) {
            visit_inorder(node->right.get());
        }
    };
    if (root) {
        visit_inorder(root);
    }

    return values;
}

// search returns the node matching value.
template <typename T>
const BSTNode<T>*
search(const BSTNode<T>* root, const T& v)
{
    if (v < root->data) {
        if (root->left) {
            return search(root->left.get(), v);
        }
        return nullptr; // Not found.
    }
    else if (v > root->data) {
        if (root->right) {
            return search(root->right.get(), v);
        }
        return nullptr; // Not found.
    }
    return root; // Exact match.
}

// min returns node having minimum value in tree.
template <typename T>
const BSTNode<T>*
min(const BSTNode<T>* root)
{
    if (root->left) {
        return min(root->left.get());
    }
    return root;
}

// max returns node having maximum value in tree.
template <typename T>
const BSTNode<T>*
max(const BSTNode<T>* root)
{
    if (root->right) {
        return max(root->right.get());
    }
    return root;
}

// before returns the node with largest data < v.
template <typename T>
const BSTNode<T>*
before(const BSTNode<T>* root, const T& v, const BSTNode<T>* prev=nullptr)
{
    if (v > root->data) {
        if (root->right) {
            // root is a potential solution, search right subtree.
            return before(root->right.get(), v, root);
        }
        return root;
    }
    else if (v < root->data) {
        if (root->left) {
            // root is not potential solution, search left subtree.
            return before(root->left.get(), v, prev);
        }
        return prev;
    }
    // Solution is either left child or previous node.
    return root->left ? before(root->left.get(), v, prev) : prev;
}

// after returns the node with smallest data > v.
template <typename T>
const BSTNode<T>*
after(const BSTNode<T>* root, const T& v, const BSTNode<T>* prev=nullptr)
{
    if (v > root->data) {
        if (root->right) {
            // root is not potential solution, search right subtree.
            return after(root->right.get(), v, prev);
        }
        return prev;
    }
    else if (v < root->data) {
        if (root->left) {
            // root is a potential solution, search left subtree.
            return after(root->left.get(), v, root);
        }
        return root;
    }
    // Solution is either right child or previous node.
    return root->right ? after(root->right.get(), v, prev) : prev;
}

}

TEST_CASE("[search]")
{
    using namespace algorithms;

    // SearchResult holds searched for value and expected result.
    using SearchResult = std::pair<int, std::optional<int>>;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::vector<SearchResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced BST.",
            {7, 3, 1, 5, 11, 9, 13},
            {
                {0, std::nullopt},
                {1, 1},
                {2, std::nullopt},
                {3, 3},
                {4, std::nullopt},
                {5, 5},
                {6, std::nullopt},
                {7, 7},
                {8, std::nullopt},
                {9, 9},
                {10, std::nullopt},
                {11, 11},
                {12, std::nullopt},
                {13, 13},
                {14, std::nullopt},
            },
        },
        {
            "Unbalanced BST.",
            {1, 7, 5, 9, 3},
            {
                {0, std::nullopt},
                {1, 1},
                {2, std::nullopt},
                {3, 3},
                {4, std::nullopt},
                {5, 5},
                {6, std::nullopt},
                {7, 7},
                {8, std::nullopt},
                {9, 9},
                {10, std::nullopt},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_bst(c.values);
        for (const auto& expect : c.expected) {
            auto rcv = search(root.get(), expect.first);
            if (expect.second == std::nullopt) {
                REQUIRE(rcv == nullptr);
            }
            else {
                REQUIRE(rcv != nullptr);
                REQUIRE(rcv->data == *expect.second);
            }
        }
    }
}

TEST_CASE("[min]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        int expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced BST.",
            {7, 3, 1, 5, 11, 9, 13},
            1,
        },
        {
            "Unbalanced BST.",
            {1, 7, 5, 9, 3},
            1,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_bst(c.values);
        auto rcv = min(root.get());
        REQUIRE(rcv != nullptr);
        REQUIRE(rcv->data == c.expected);
    }
}

TEST_CASE("[max]")
{
    using namespace algorithms;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        int expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced BST.",
            {7, 3, 1, 5, 11, 9, 13},
            13,
        },
        {
            "Unbalanced BST.",
            {1, 7, 5, 9, 3},
            9,
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_bst(c.values);
        auto rcv = max(root.get());
        REQUIRE(rcv != nullptr);
        REQUIRE(rcv->data == c.expected);
    }
}

TEST_CASE("[before]")
{
    using namespace algorithms;

    // BeforeResult holds searched for value and expected result.
    using BeforeResult = std::pair<int, std::optional<int>>;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::vector<BeforeResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced BST.",
            {7, 3, 1, 5, 11, 9, 13},
            {
                {0, std::nullopt},
                {1, std::nullopt},
                {2, 1},
                {3, 1},
                {4, 3},
                {5, 3},
                {6, 5},
                {7, 5},
                {8, 7},
                {9, 7},
                {10, 9},
                {11, 9},
                {12, 11},
                {13, 11},
                {14, 13},
            },
        },
        {
            "Unbalanced BST.",
            {1, 7, 5, 9, 3},
            {
                {0, std::nullopt},
                {1, std::nullopt},
                {2, 1},
                {3, 1},
                {4, 3},
                {5, 3},
                {6, 5},
                {7, 5},
                {8, 7},
                {9, 7},
                {10, 9},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_bst(c.values);
        for (const auto& expect : c.expected) {
            auto rcv = before(root.get(), expect.first);
            if (expect.second == std::nullopt) {
                REQUIRE(rcv == nullptr);
            }
            else {
                REQUIRE(rcv != nullptr);
                REQUIRE(rcv->data == *expect.second);
            }
        }
    }
}

TEST_CASE("[after]")
{
    using namespace algorithms;

    // AfterResult holds searched for value and expected result.
    using AfterResult = std::pair<int, std::optional<int>>;

    struct test_case
    {
        std::string name;
        std::vector<int> values;
        std::vector<AfterResult> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Balanced BST.",
            {7, 3, 1, 5, 11, 9, 13},
            {
                {0, 1},
                {1, 3},
                {2, 3},
                {3, 5},
                {4, 5},
                {5, 7},
                {6, 7},
                {7, 9},
                {8, 9},
                {9, 11},
                {10, 11},
                {11, 13},
                {12, 13},
                {13, std::nullopt},
                {14, std::nullopt},
            },
        },
        {
            "Unbalanced BST.",
            {1, 7, 5, 9, 3},
            {
                {0, 1},
                {1, 3},
                {2, 3},
                {3, 5},
                {4, 5},
                {5, 7},
                {6, 7},
                {7, 9},
                {8, 9},
                {9, std::nullopt},
                {10, std::nullopt},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        auto root = make_bst(c.values);
        for (const auto& expect : c.expected) {
            auto rcv = after(root.get(), expect.first);
            if (expect.second == std::nullopt) {
                REQUIRE(rcv == nullptr);
            }
            else {
                REQUIRE(rcv != nullptr);
                REQUIRE(rcv->data == *expect.second);
            }
        }
    }
}
