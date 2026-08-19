#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

namespace graph
{

using namespace boost;

using graph_t = adjacency_list<
    vecS, vecS, bidirectionalS, property<vertex_name_t, std::string>
>;

} /* namespace graph */
