#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <unordered_set>
#include <vector>

#include "graph.hpp"
#include "reverse-graph.hpp"
#include "transitive-closure.hpp"

namespace graph
{

namespace detail
{
// я так рад, что живу в эпоху нейросетей и эти чудные строки, за меня может
// написать мой чудный китайский друг

struct vertex_hash
{
  std::size_t
  operator ()(
      graph_t::vertex_descriptor v
  ) const
  { return std::hash<graph_t::vertex_descriptor>()(v); }
};

struct vertex_equal
{
  bool
  operator ()(
      graph_t::vertex_descriptor a, graph_t::vertex_descriptor b
  ) const
  { return a == b; }
};

using vertex_set =
    std::unordered_set<graph_t::vertex_descriptor, vertex_hash, vertex_equal>;
}  // namespace detail

inline
graph_t
get_dominators_graph(
    graph_t const& g
)
{
  // reverse graph - for comfortable work with vertex preds
  graph_t reversed_graph = get_reversed_graph(g);
  copy_vertices_names(g, reversed_graph);

  auto&&                          vertices_range = vertices(reversed_graph);
  std::vector<detail::vertex_set> dominators(num_vertices(g));

  // entry is its own dominator
  auto&& entry = *vertices_range.first;
  dominators[entry].insert(entry);

  auto no_entry_vertices_range = make_iterator_range(
      std::next(vertices_range.first), vertices_range.second
  );

  // initialize dominators as all preds for every vertex without entry
  for (auto&& v : no_entry_vertices_range)
  {
    auto&& v_dominators = dominators[v];
    std::copy(
        vertices_range.first,
        vertices_range.second,
        std::inserter(v_dominators, v_dominators.begin())
    );
  }

  bool changed = true;

  while (changed)
  {
    changed = false;
    for (auto v : no_entry_vertices_range)
    {
      // intersect all preds dominators sets
      auto preds = adjacent_vertices(v, reversed_graph);
      assert(preds.first != preds.second && "unexpected graph was genereted");
      // initialize new_dominators
      // initialize value - all dominators of first pred
      detail::vertex_set new_dominators = dominators[*preds.first];
      // range without first pred, because its dominators already used
      auto no_first_element_range =
          make_iterator_range(std::next(preds.first), preds.second);
      for (auto pred : no_first_element_range)
      {
        detail::vertex_set tmp_intersection;
        auto&&             pred_dominators = dominators[pred];

        // жаль что здесь не std::set, а std::unordered_set, а так было бы
        // приятнее здесь жить
        //
        // std::set_intersection(
        //    new_dominators.begin(), new_dominators.end(),
        //     pred_dominators.begin(), pred_dominators.end(),
        //     std::inserter(intersection, intersection.begin())
        // );

        // add d to dominators only if it contains in new_dominators and in
        // pred_dominators
        for (auto d : new_dominators)
        {
          if (!pred_dominators.contains(d)) continue;
          tmp_intersection.insert(d);
        }

        // update new_dominators (was intersected with preds dominators)
        new_dominators = std::move(tmp_intersection);
      }

      // vertex is its own dominator
      new_dominators.insert(v);

      if (new_dominators == dominators[v]) continue;
      // update vertexs dominators if it was changed
      dominators[v] = std::move(new_dominators);
      changed       = true;
    }
  }

  graph_t dominator_graph = get_reversed_graph(g);
  copy_vertices_names(g, dominator_graph);

  for (auto v : make_iterator_range(vertices(dominator_graph)))
    clear_out_edges(v, dominator_graph);

  for (auto v : make_iterator_range(vertices_range))
  {
    for (auto dom : dominators[v])
    {
      if (dom == v) continue;  // ignore self-domination
      add_edge(dom, v, dominator_graph);
    }
  }

  return dominator_graph;
}

}  // namespace graph
