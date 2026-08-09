#include <cmath>
#include <cstddef>
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <filesystem>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/subgraph.hpp>

using namespace boost;
using graph_t = boost::adjacency_list<>;

class GraphRandomizer
{
private:
    std::random_device rd;
    std::mt19937 rg;
    size_t min_vertex_number;
    size_t max_vertex_number;
    size_t vertex_number;
    size_t max_edges_number;
    size_t adequate_number_of_edges;

public:
    GraphRandomizer(size_t min_vertex_number = 10, size_t max_vertex_number = 100)
    : rg(rd()), min_vertex_number(min_vertex_number), max_vertex_number(max_vertex_number), vertex_number(get_random_unsigned_value_in_range(min_vertex_number, max_vertex_number)),
      max_edges_number(vertex_number*(vertex_number-1)/2),
      adequate_number_of_edges(get_adequate_number_of_edges(vertex_number))
    {} 

private:
    inline void update_random_values()
    {
      vertex_number = get_random_unsigned_value_in_range(min_vertex_number, max_vertex_number);
      max_edges_number = vertex_number*(vertex_number-1)/2;
      adequate_number_of_edges = get_adequate_number_of_edges(vertex_number);
    }

    inline bool will_generate_edge()
    {
      return get_random_unsigned_value_less_than(max_edges_number) < adequate_number_of_edges;
    }

    inline unsigned get_random_unsigned_value_less_than(unsigned n)
    {
      return get_random_unsigned_value_in_range(0, n);
    }

    inline unsigned get_random_unsigned_value_in_range(unsigned min, unsigned max)
    {
      return std::uniform_int_distribution<size_t>{min, max}(rg);
    }

    static inline size_t get_adequate_number_of_edges(size_t vetrtex_number)
    {
      // n/2 * (ln(n) + ln(ln(n))) - with this edges number,
      // graph became a connected and have cycles with high probability

      double n = vetrtex_number;
      double ln_n = std::log(n);
      return n * (ln_n + std::log(ln_n)) / 2;
    }

public:
    inline graph_t get_random_graph()
    {
      graph_t graph;

      std::vector<decltype(add_vertex(graph))> vertices{vertex_number};

      for (auto&& it = 0lu, ite = vertex_number; it != ite; ++it)
        vertices.push_back(add_vertex(graph));

      for (auto&& it = 0lu, ite = vertices.size(); it != ite; ++it)
      {
        for (auto&& jt = it + 1; jt < ite; ++jt)
        {
          if (!will_generate_edge()) continue;
          add_edge(vertices[it], vertices[jt], graph);
        }
      }

      update_random_values();
      return graph;
    }
};

inline void write_graph(graph_t const & graph, std::filesystem::path const& path)
{
  std::string name = path.filename();
  std::ofstream ofs{name+".dot"};
  write_graphviz(ofs, graph);
  std::string dot_command = "dot -Tsvg "+name+".dot > "+name+".svg";
  std::system(dot_command.c_str());
  std::cerr << "[info] graph was dumped in \""<<name<<".svg\""<<"\n";
}

// int main(int argc, char* argv[])
// {
//   GraphRandomizer graph_randomizer{0, 10};

//   auto&& graph = graph_randomizer.get_random_graph();
//   write_graph(graph, "graph");
  
//   return 0;
// }
 
