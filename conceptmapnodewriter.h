#ifndef CONCEPTMAPNODEWRITER_H
#define CONCEPTMAPNODEWRITER_H

#include "conceptmapgraphtypes.h"
#include "conceptmapnode.h"

namespace ribi {
namespace cmap {

//Very similar to bundled_vertices_writer
class node_writer {
public:
  node_writer(
    const ConceptMap& g
  ) : m_g{g}
  {

  }
  template <class vertex_descriptor>
  void operator()(
    std::ostream& out,
    const vertex_descriptor& vd
  ) const noexcept {
    const Node& node = m_g[vd];
    out << "[label=\""
      << node //Can be Graphviz unfriendly
      << "\"]"
    ;
  }
private:
  ConceptMap m_g;
};

inline node_writer
make_node_writer(
  const ConceptMap& g
)
{
  return node_writer(g);
}


} //~namespace ribi
} //~namespace cmap

#endif // CONCEPTMAPNODEWRITER_H
