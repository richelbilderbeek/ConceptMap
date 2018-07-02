#ifndef CONCEPTMAPGRAPHTYPE_H
#define CONCEPTMAPGRAPHTYPE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "conceptmapnode.h"
#include "conceptmapedge.h"

namespace ribi {
namespace cmap {

using ConceptMap = boost::adjacency_list
<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  Node,
  Edge
>;
using VertexDescriptor = boost::graph_traits<ConceptMap>::vertex_descriptor;
static_assert(sizeof(VertexDescriptor) == sizeof(VertexDescriptor&),"On university computer");
using EdgeDescriptor = boost::graph_traits<ConceptMap>::edge_descriptor;

} //~namespace cmap
} //~namespace ribi



#endif // CONCEPTMAPGRAPHTYPE_H
