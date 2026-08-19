#pragma once

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <cassert>
#include <limits>

#include "copy-names.hpp"
#include "graph.hpp"
#include "reverse-graph.hpp"

namespace graph
{
namespace detail
{
inline
graph_t::vertex_descriptor
get_immediate_dominator_stupid(
    graph_t::vertex_descriptor v, graph_t const& graph,
    graph_t const& reversed_dominators_graph
)  // main idea: the closest to vertex dominator is immediate dominator. so, we
   // can just run bfs up the tree and first dominator, which we'll found, will
   // be a immidiate dominator
{
  auto vertex_dominators_range = adjacent_vertices(
      v, reversed_dominators_graph
  );  // expected that self-domination is excluded
  auto dominators_num = std::distance(
      vertex_dominators_range.first, vertex_dominators_range.second
  );
  assert(dominators_num != 0);

  // if (dominators_num == 1) return *vertex_dominators_range.first;

  std::vector<unsigned> distances(
      num_vertices(graph), std::numeric_limits<unsigned>::max()
  );

  // maybe smarter: stop bfs when first dominator was found
  // problem: check that visited vertex is a dominator

  // possible improve of current algorithm:
  // to not run bfs from every node, maybe done somehting like this:
  // transitive closure of graph, where every edge has weight (minimal distance
  // between vertices)
  breadth_first_search(
      graph,
      v,
      visitor(
          make_bfs_visitor(record_distances(distances.data(), on_tree_edge()))
      )
  );

  unsigned min_dominator_distance =
      std::numeric_limits<decltype(min_dominator_distance)>::max();
  graph_t::vertex_descriptor immediate_dominator =
      v;  // v is just initializaing value, which guareanterd will not be a
          // immediate dominator
  for (auto dominator : make_iterator_range(vertex_dominators_range))
  {
    auto dominator_distance = distances[dominator];
    if (dominator_distance > min_dominator_distance) continue;
    min_dominator_distance = dominator_distance;
    immediate_dominator    = dominator;
  }
  assert(min_dominator_distance != std::numeric_limits<unsigned>::max());
  assert(immediate_dominator != v);
  return immediate_dominator;
}
}  // namespace detail

inline
graph_t
get_dominators_tree_stupid(
    graph_t const& graph,
    graph_t const&
        dominators_graph  // expected graph with ignored self-domination
)
{
  auto&& reversed_graph            = get_reversed_graph(graph);
  auto&& reversed_dominators_graph = get_reversed_graph(dominators_graph);
  auto&& vertices_range            = vertices(reversed_dominators_graph);
  std::vector<graph_t::vertex_descriptor> immediate_dominators(
      num_vertices(dominators_graph)
  );

  // exlude entry, because it doesn't have immediate dominator
  auto no_entry_range = make_iterator_range(
      std::next(vertices_range.first), vertices_range.second
  );

  for (auto v : no_entry_range)
    immediate_dominators[v] = detail::get_immediate_dominator_stupid(
        v, reversed_graph, reversed_dominators_graph
    );

  graph_t dominators_tree(num_vertices(dominators_graph));
  copy_vertices_names(dominators_graph, dominators_tree);

  auto dominators_tree_vertices_range = vertices(dominators_tree);
  auto no_entry_dominators_tree_range = make_iterator_range(
      std::next(dominators_tree_vertices_range.first),
      dominators_tree_vertices_range.second
  );

  for (auto v : no_entry_dominators_tree_range)
    add_edge(immediate_dominators[v], v, dominators_tree);

  return dominators_tree;
}
}  // namespace graph
