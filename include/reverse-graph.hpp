#pragma once

#include "graph.hpp"

namespace graph
{
inline
graph_t
get_reversed_graph(
    graph_t const& g
)
{
  graph_t reversed(num_vertices(g));
  for (auto e : boost::make_iterator_range(boost::edges(g)))
  {
    add_edge(boost::target(e, g), boost::source(e, g), reversed);
  }
  return reversed;
}
}  // namespace graph
