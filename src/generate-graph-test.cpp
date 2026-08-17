#include "generate-graph.hpp"
#include "graph.hpp"
#include "write-graph.hpp"

int main()
{

    auto &&graph = graph::get_random_graph();
    graph::write_graph(graph, "generate-graph-test");

    return 0;
}
