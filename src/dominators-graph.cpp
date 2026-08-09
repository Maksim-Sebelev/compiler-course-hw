#include "dominator-graph.hpp"
#include "write-graph.hpp"
#include "generate-graph.hpp"

int main()
{
  auto&& random_graph = graph::get_random_graph();
  graph::write_graph(random_graph, "original-graph");

  auto&& dominators_graph = graph::get_dominators_graph(random_graph);
  graph::write_graph(dominators_graph , "dominators-graph");

  return 0;
}
