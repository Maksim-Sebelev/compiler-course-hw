#include <boost/graph/subgraph.hpp>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "graph.hpp"

namespace graph
{

class GraphRandomizer
{
private:
  std::random_device rd;
  std::mt19937       rg;
  size_t             vertex_number;
  size_t             max_edges_number;
  size_t             adequate_number_of_edges;

public:
  GraphRandomizer() : rg(rd()) {}

private:
  inline
  void
  update_random_values(
      size_t min_vertex_number, size_t max_vertex_number
  )
  {
    vertex_number = get_random_unsigned_value_in_range(
        min_vertex_number, max_vertex_number
    );
    max_edges_number         = vertex_number * (vertex_number - 1) / 2;
    adequate_number_of_edges = get_adequate_number_of_edges(vertex_number);
  }

  inline
  bool
  will_generate_edge()
  {
    return get_random_unsigned_value_less_than(max_edges_number) <
           adequate_number_of_edges;
  }

  inline
  unsigned
  get_random_unsigned_value_less_than(
      unsigned n
  )
  { return get_random_unsigned_value_in_range(0, n); }

  inline
  unsigned
  get_random_unsigned_value_in_range(
      unsigned min, unsigned max
  )
  { return std::uniform_int_distribution<size_t>{min, max}(rg); }

  static inline
  size_t
  get_adequate_number_of_edges(
      size_t vetrtex_number
  )
  {
    // n/2 * (ln(n) + ln(ln(n))) - with this edges number,
    // graph became a connected and have cycles with high probability

    double n    = vetrtex_number;
    double ln_n = std::log(n);
    return n * (ln_n + std::log(ln_n)) / 2;
  }

public:
  inline
  graph_t
  get_random_graph(
      size_t min_vertex_number = 10, size_t max_vertex_number = 100
  )
  {
    update_random_values(min_vertex_number, max_vertex_number);

    graph_t graph;

    std::vector<decltype(add_vertex(
        property<vertex_name_t, std::string>("PIZDA"), graph
    ))>
        vertices;
    vertices.reserve(vertex_number);

    // add ENTRY
    vertices.push_back(
        add_vertex(property<vertex_name_t, std::string>("ENTRY"), graph)
    );

    // add vertices 1..(n-1)
    for (auto &&it = 1lu, ite = vertex_number - 1; it < ite; ++it)
      vertices.push_back(add_vertex(
          property<vertex_name_t, std::string>(std::to_string(it)), graph
      ));

    // add EXIT
    vertices.push_back(
        add_vertex(property<vertex_name_t, std::string>("EXIT"), graph)
    );

    auto&& entry = vertices[0];
    auto&& exit  = vertices.back();

    for (auto &&it = 1lu, ite = vertex_number - 1; it < ite; ++it)
    {
      if (not will_generate_edge()) continue;
      add_edge(entry, vertices[it], graph);
    }

    if (out_degree(entry, graph) == 0)
      add_edge(
          entry,
          vertices[get_random_unsigned_value_in_range(1, vertex_number - 2)],
          graph
      );

    for (auto it = 1lu, ite = vertex_number - 1; it < ite; ++it)
    {
      auto&& vi = vertices[it];
      for (auto jt = it + 1, jte = vertex_number - 1; jt < ite; ++jt)
      {
        auto&& vj = vertices[jt];
        if (will_generate_edge()) add_edge(vi, vj, graph);
        if (will_generate_edge()) add_edge(vj, vi, graph);
      }

      // this situations are almost impossible at big graphs
      // for no separated vertices
      if (out_degree(vi, graph) == 0) add_edge(vi, exit, graph);
      if (in_degree(vi, graph) == 0) add_edge(entry, vi, graph);
    }

    for (auto it = 1lu, ite = vertex_number - 1; it < ite; ++it)
    {
      if (not will_generate_edge()) continue;
      auto&& vi = vertices[it];
      if (edge(vi, exit, graph).second) continue;
      add_edge(vi, exit, graph);
    }

    if (in_degree(exit, graph) == 0)
      add_edge(
          vertices[get_random_unsigned_value_in_range(1, vertex_number - 2)],
          exit,
          graph
      );

    return graph;
  }
};

inline
graph_t
get_random_graph(
    unsigned min_vertex_number = 3, unsigned max_vertex_number = 10
)
{
  return GraphRandomizer{}.get_random_graph(
      min_vertex_number, max_vertex_number
  );
}

} /* namespace graph */

// USAGE EXAMPLE:
/*
#include "generate-graph.hpp"
#include "write-graph.hpp"

int main(int argc, char* argv[])
{
  GraphRandomizer graph_randomizer{0, 10};

  auto&& graph = graph_randomizer.get_random_graph();
  write_graph(graph, "graph");

  return 0;
}
*/
