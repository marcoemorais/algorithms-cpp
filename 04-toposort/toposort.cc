#include <algorithm>
#include <cstddef>
#include <forward_list>
#include <iterator>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
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

// Graph is an adjacency list representation of an unweighted graph.
struct Graph
{
    using VertexList = std::forward_list<VertexID>;

    Graph() = default;

    void add_edge(const VertexID& from, const VertexID& to)
    {
        auto vlist = vertices.find(from);
        if (vlist != std::end(vertices)) {
            // Insert `to` if not already in the list.
            auto vto = std::find(std::begin(vlist->second),
                                 std::end(vlist->second),
                                 to);
            if (vto == std::end(vlist->second)) {
                vlist->second.push_front(to);
            }
        }
        else {
            vertices[from] = VertexList{to}; // New list.
        }
        if (vertices.count(to) < 1) {
            // Add an empty list for `to` if not already in graph.
            vertices[to] = VertexList{};
        }
    }

    std::unordered_map<VertexID, VertexList> vertices;
};

// dfs performs a depth first search of the graph from start.
template <typename VisitFunc>
void
dfs(const Graph& graph, const VertexID& start,
    std::unordered_set<VertexID>& visited,
    VisitFunc visit)
{
    visited.insert(start); // Don't visit start again.
    for (const auto& vto : graph.vertices.at(start)) {
        if (visited.count(vto) < 1) {
            dfs(graph, vto, visited, visit);
        }
    }
    // Crucial to perform the visit after all other
    // vertices findable from start are visited.
    visit(start);
}

// toposort returns the topological order of each vertex in graph.
std::unordered_map<VertexID, std::size_t>
toposort(const Graph& graph)
{
    std::unordered_map<VertexID, std::size_t> order;
    std::unordered_set<VertexID> visited;
    std::size_t label = graph.vertices.size();

    auto visit_func = [&order, &label](const VertexID& v) {
        // Assign label to vertex and decrement.
        order[v] = label;
        label -= 1;
    };

    for (const auto& vstart : graph.vertices) {
        if (visited.count(vstart.first)) {
            continue; // Skip previously visited vertices.
        }
        dfs(graph, vstart.first, visited, visit_func);
    }

    return order;
}

}

TEST_CASE("[toposort]")
{
    using namespace algorithms;

    using AddEdgeParams = std::tuple<VertexID, VertexID>;

    struct test_case
    {
        std::string name;
        std::vector<AddEdgeParams> edges;
        std::unordered_map<VertexID, std::size_t> expected;
    };

    std::vector<test_case> test_cases{
        {
            "DAG1",
            {
                {100, 110},
                {100, 120},
                {110, 200},
                {120, 200},
                {120, 220},
                {200, 300},
                {220, 350},
                {300, 400},
                {350, 400},
            },
            {
                {100, 1},
                {110, 2},
                {120, 3},
                {200, 4},
                {220, 5},
                {300, 7},
                {350, 6},
                {400, 8},
            },
        },
        {
            "DAG2",
            {
                {2, 1},
                {3, 1},
                {4, 2},
                {5, 1},
                {6, 2},
                {6, 3},
                {7, 1},
                {9, 3},
                {8, 4},
                {10, 5},
                {10, 2},
            },
            {
                {6, 1},
                {7, 2},
                {9, 3},
                {3, 4},
                {8, 5},
                {4, 6},
                {10, 7},
                {5, 8},
                {2, 9},
                {1, 10},
            },
        },
        {
            "DAG3",
            {
                {1, 2},
                {1, 3},
                {2, 3},
                {2, 4},
                {3, 5},
                {3, 6},
                {5, 4},
                {6, 5},
                {7, 1},
                {7, 6},
            },
            {
                {1, 2},
                {2, 3},
                {3, 4},
                {4, 7},
                {5, 6},
                {6, 5},
                {7, 1},
            },
        },
    };

    for (const auto& c : test_cases) {
        INFO(c.name);
        Graph g;
        for (const auto& e : c.edges) {
            g.add_edge(std::get<0>(e), std::get<1>(e));
        }
        auto rcv = toposort(g);
        REQUIRE(rcv == c.expected);
    }
}
