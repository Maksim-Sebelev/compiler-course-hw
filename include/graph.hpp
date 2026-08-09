#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/subgraph.hpp>

namespace graph
{
using namespace boost;
using graph_t = boost::adjacency_list<vecS, vecS, bidirectionalS, 
    property<vertex_name_t, std::string>>;
} /* namespace graph */

