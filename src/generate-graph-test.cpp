#include "graph.hpp"
#include "write-graph.hpp"
#include "generate-graph.hpp"

int main()
{

  auto&& graph = graph::get_random_graph();
  graph::write_graph(graph, "generate-graph-test");

  return 0;
}
