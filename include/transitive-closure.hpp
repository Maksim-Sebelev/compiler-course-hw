#pragma once

#include <boost/graph/transitive_closure.hpp>

#include "copy-names.hpp"
#include "graph.hpp"

namespace graph
{

inline
graph_t
get_transitive_closure(
    graph_t const& g
)
{
  graph_t tc;
  transitive_closure(g, tc);  // but why names are not copying :(
  copy_vertices_names(g, tc);
  return tc;
}

}  // namespace graph
