#include <boost/graph/graphviz.hpp>
#include <filesystem>
#include <string>

#include "graph.hpp"

namespace graph
{

inline
void
write_graph(
    graph_t const& graph, std::filesystem::path const& path
)
{
  std::string   name = path.filename();
  std::ofstream ofs{name + ".dot"};
  write_graphviz(ofs, graph, make_label_writer(get(vertex_name, graph)));
  std::string dot_command = "dot -Tsvg " + name + ".dot > " + name + ".svg";
  std::system(dot_command.c_str());
  std::cerr << "[info] graph was dumped in \"" << name << ".svg\"" << "\n";
}

} /* namespace graph */
