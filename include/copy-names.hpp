#include "graph.hpp"

namespace graph
{
inline
void
copy_vertices_names(
    graph_t const& g, graph_t& cg
)
{
  auto name_map_g  = get(vertex_name, g);
  auto name_map_cg = get(vertex_name, cg);

  auto vertices_g  = vertices(g);
  auto vertices_cg = vertices(cg);

  for (auto vg = vertices_g.first, vcg = vertices_cg.first;
       vg != vertices_g.second && vcg != vertices_cg.second;
       ++vg, ++vcg)
  {
    name_map_cg[*vcg] = name_map_g[*vg];
  }
}
}  // namespace graph
