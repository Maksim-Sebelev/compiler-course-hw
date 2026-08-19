#include "dominators-tree.hpp"

#include "dominator-graph.hpp"
#include "generate-graph.hpp"
#include "graph.hpp"
#include "write-graph.hpp"

int
main()
{
  auto&& graph            = graph::get_random_graph();
  auto&& dominators_graph = graph::get_dominators_graph(graph);
  auto&& dominators_tree =
      graph::get_dominators_tree_stupid(graph, dominators_graph);

  graph::write_graph(graph, "original-graph");
  graph::write_graph(dominators_graph, "dominators-graph");
  graph::write_graph(dominators_tree, "dominators-tree");

  return 0;
}
