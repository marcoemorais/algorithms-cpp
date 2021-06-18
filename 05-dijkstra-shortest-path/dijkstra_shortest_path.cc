#include <algorithm>
#include <cstddef>
#include <forward_list>
#include <functional>
#include <limits>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#if __has_include("algorithms/print.h")
#   include "algorithms/print.h"
#endif

namespace algorithms
{

using VertexID = int;

using EdgeWeight = float;

struct key_not_found_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct heap_empty_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// Dijkstra's algorithm requires a special purpose heap
// that supports deleting and reinserting elements from
// anywhere in the heap, not just at the root.
//
// In order to support this requirement, we implement
// a custom heap that contains a hashtable that stores
// the current position of each element in the heap.
//
// The erase member function supports removing elements
// from the heap matching the key passed as input.
//
template <typename Key,
          typename Value,
          typename Less = std::less<Value>,
          typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class Heap
{
public:
    using HeapEntry = std::pair<Key, Value>;

    Heap() = default;

    // push adds Key ordered by Value to heap.
    void push(const Key& k, const Value& v)
    {
        tree.emplace_back(k, v);
        hashtable[k] = tree.size()-1;
        bubbleUp(tree.size()-1);
    }

    // contains returns true when Key is in the heap.
    bool contains(const Key& k) const
    {
        return hashtable.count(k) > 0;
    }

    // value returns Value associated with Key in the heap.
    const Value& value(const Key& k) const
    {
        return tree[hashtable.at(k)].second;
    }

    // erase removes Key from anywhere in heap.
    void erase(const Key& k)
    {
        if (hashtable.count(k) < 1) {
            throw key_not_found_error{"erase with non-existent key"};
        }
        if (size() == 1) {
            hashtable.erase(k);
            tree.resize(tree.size()-1);
        }
        else {
            // rmind points to the key being deleted.
            // Overwrite key with last value in the heap.
            Value rmind = hashtable[k];
            tree[rmind] = tree[tree.size()-1];
            tree.resize(tree.size()-1);
            // Update the hashtable and restore heap property.
            hashtable[tree[rmind].first] = rmind;
            hashtable.erase(k);
            bubbleDown(rmind);
        }
    }

    // top returns but does not remove element at top of heap.
    const Key& top() const
    {
        if (size() < 1) {
            throw heap_empty_error{"top from empty heap"};
        }
        return tree[1].first;
    }

    // pop removes but does not return element at top of heap.
    void pop()
    {
        if (size() < 1) {
            throw heap_empty_error{"pop from empty heap"};
        }
        Key k = tree[1].first; // Since key is being deleted, make a copy.
        erase(k);
    }

    // size returns number of elements in heap.
    std::size_t size() const
    {
        return tree.size()-1; // Ignore sentinel entry.
    }

private:
    // tree contains the elements of the heap stored by Value.
    std::vector<HeapEntry> tree{HeapEntry{0, 0}};

    // hashtable contains the elements of heap stored by Key.
    std::unordered_map<Key, std::size_t, Hash, KeyEqual> hashtable;

    // less is the comparison function used to maintain heap order.
    Less less;

    // hasher is the hash function used to hash Key to bucket.
    Hash hasher;

    // equals is the equality function for Keys in same bucket.
    KeyEqual equals;

    // bubbleUp maintains the heap property of parents of ind.
    void bubbleUp(std::size_t ind)
    {
        std::size_t parent = ind/2;
        if (parent < 1) {
            return; // Already at root.
        }
        else {
            if (less(tree[ind].second, tree[parent].second)) {
                // Swap heap element positions and update hashtable.
                std::swap(tree[ind], tree[parent]);
                hashtable[tree[ind].first] = ind;
                hashtable[tree[parent].first] = parent;
                bubbleUp(parent);
            }
        }
    }

    // bubbleDown maintains the heap property of children of ind.
    void bubbleDown(std::size_t ind)
    {
        std::size_t lchild = ind*2;
        if (lchild < tree.size()) {
            // Find index of the smaller of the children.
            std::size_t minchild = lchild;
            if (lchild+1 < tree.size()) {
                if (less(tree[lchild+1].second, tree[lchild].second)) {
                    minchild = lchild+1;
                }
            }
            if (less(tree[minchild].second, tree[ind].second)) {
                // Swap heap element positions and update hashtable.
                std::swap(tree[minchild], tree[ind]);
                hashtable[tree[minchild].first] = minchild;
                hashtable[tree[ind].first] = ind;
                bubbleDown(minchild);
            }
        }
    }
};

struct negative_edge_weight : std::invalid_argument
{
    using std::invalid_argument::invalid_argument;
};

// Graph is an adjacency list representation of a weighted graph.
struct Graph
{
    using VertexListEntry = std::pair<VertexID, EdgeWeight>;

    using VertexList = std::forward_list<VertexListEntry>;

    Graph() = default;

    void add_edge(const VertexID& from, const VertexID& to, const EdgeWeight& w)
    {
        if (w < 0) {
            throw negative_edge_weight{"add_edge with negative weight"};
        }
        auto vit = vertices.find(from);
        if (vit != std::end(vertices)) {
            vit->second.emplace_front(to, w);
        }
        else {
            vertices[from] = VertexList{VertexListEntry{to, w}};
        }
        if (vertices.count(to) < 1) {
            vertices[to] = VertexList{}; // Add empty `to` list.
        }
    }

    std::unordered_map<VertexID, VertexList> vertices;
};

constexpr EdgeWeight INF = std::numeric_limits<EdgeWeight>::max();

// shortest_path returns the single source shortest path from start.
std::unordered_map<VertexID, EdgeWeight>
shortest_path(const Graph& g, const VertexID& start)
{
    // Initialize all vertices, except start, with infinite cost path.
    std::unordered_map<VertexID, EdgeWeight> cost;
    for (const auto& vit : g.vertices) {
        cost[vit.first] = vit.first != start ? INF : 0;
    }

    // Heap is used to pick shortest path vertex along frontier.
    Heap<VertexID, EdgeWeight> vertices;
    for (const auto& cit : cost) {
        vertices.push(cit.first, cit.second);
    }

    while (vertices.size()) {
        // Remove vertex having shortest path from heap and update cost.
        VertexID vfrom = vertices.top();
        cost[vfrom] = vertices.value(vfrom);
        vertices.pop();
        for (const auto& vit : g.vertices.at(vfrom)) {
            // If path from vfrom results in a shorter path to one of
            // adjacent vertices still in heap, then update cost in heap.
            if (vertices.contains(vit.first)) {
                auto vcost = cost[vfrom] + vit.second;
                if (vcost < vertices.value(vit.first)) {
                    vertices.erase(vit.first);
                    vertices.push(vit.first, vcost);
                }
            }
        }
    }

    return cost;
}

}

TEST_CASE("[Heap]")
{
    using namespace algorithms;

    using Key = int;
    using Value = float;
    Heap<Key, Value> heap;

    REQUIRE(heap.size() == 0);

    heap.push(2, 20);
    REQUIRE(heap.top() == 2);
    REQUIRE(heap.contains(2) == true);
    REQUIRE(heap.value(2) == 20);
    REQUIRE(heap.size() == 1);

    heap.push(3, 30);
    REQUIRE(heap.top() == 2);
    REQUIRE(heap.contains(3) == true);
    REQUIRE(heap.value(3) == 30);
    REQUIRE(heap.size() == 2);

    heap.push(1, 10);
    REQUIRE(heap.top() == 1);
    REQUIRE(heap.contains(1) == true);
    REQUIRE(heap.value(1) == 10);
    REQUIRE(heap.size() == 3);

    heap.push(4, 40);
    REQUIRE(heap.top() == 1);
    REQUIRE(heap.contains(4) == true);
    REQUIRE(heap.value(4) == 40);
    REQUIRE(heap.size() == 4);

    heap.pop();
    REQUIRE(heap.top() == 2);
    REQUIRE(heap.contains(1) == false);
    REQUIRE(heap.size() == 3);

    heap.erase(2);
    REQUIRE(heap.top() == 3);
    REQUIRE(heap.contains(2) == false);
    REQUIRE(heap.size() == 2);

    heap.push(2, 20);
    REQUIRE(heap.top() == 2);
    REQUIRE(heap.contains(2) == true);
    REQUIRE(heap.value(2) == 20);
    REQUIRE(heap.size() == 3);

    heap.erase(4);
    REQUIRE(heap.top() == 2);
    REQUIRE(heap.contains(4) == false);
    REQUIRE(heap.size() == 2);

    heap.erase(2);
    REQUIRE(heap.top() == 3);
    REQUIRE(heap.contains(2) == false);
    REQUIRE(heap.size() == 1);

    heap.erase(3);
    REQUIRE(heap.contains(3) == false);
    REQUIRE(heap.size() == 0);
}

TEST_CASE("[shortest_path]")
{
    using namespace algorithms;

    using AddEdgeParams = std::tuple<VertexID, VertexID, EdgeWeight>;

    struct test_case
    {
        std::string name;
        std::vector<AddEdgeParams> edges;
        bool directed;
        VertexID start;
        std::unordered_map<VertexID, EdgeWeight> expected;
    };

    std::vector<test_case> test_cases{
        {
            "Directed acyclic graph.",
            {
                {1, 2, 1},
                {1, 3, 4},
                {2, 3, 2},
                {2, 4, 6},
                {3, 4, 3},
            },
            1,
            true,
            {
                {1, 0},
                {2, 1},
                {3, 3},
                {4, 6},
            },
        },
        {
            "Undirected acyclic graph.",
            {
                {0, 1, 4},
                {0, 7, 8},
                {1, 2, 8},
                {1, 7, 11},
                {2, 3, 7},
                {2, 8, 2},
                {2, 5, 4},
                {3, 4, 9},
                {3, 5, 14},
                {4, 5, 10},
                {5, 6, 2},
                {6, 7, 1},
                {6, 8, 6},
                {7, 8, 7},
            },
            false,
            0,
            {
                {0, 0},
                {1, 4},
                {2, 12},
                {3, 19},
                {4, 21},
                {5, 11},
                {6, 9},
                {7, 8},
                {8, 14},
            },
        },
    };

    for (const auto& c :test_cases) {
        INFO(c.name);
        Graph g;
        for (const auto& e : c.edges) {
            g.add_edge(std::get<0>(e), std::get<1>(e), std::get<2>(e));
            if (!c.directed) {
                g.add_edge(std::get<1>(e), std::get<0>(e), std::get<2>(e));
            }
        }
        auto rcv = shortest_path(g, c.start);
        REQUIRE(rcv == c.expected);
    }
}
