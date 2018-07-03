#ifndef CONCEPTMAPEDGEWRITER_H
#define CONCEPTMAPEDGEWRITER_H

#include "conceptmapgraphtypes.h"
#include "conceptmapedge.h"

namespace ribi {
namespace cmap {

//Very similar to bundled_vertices_writer
class edge_writer {
public:
  edge_writer(
    const ConceptMap& g
  ) : m_g{g}
  {

  }
  template <class edge_descriptor>
  void operator()(
    std::ostream& out,
    const edge_descriptor& ed
  ) const noexcept {
    const Edge& edge = m_g[ed];
    out << "[label=\""
      << edge //Can be Graphviz unfriendly
      << "\"]"
    ;
  }
private:
  ConceptMap m_g;
};

inline edge_writer
make_edge_writer(
  const ConceptMap& g
)
{
  return edge_writer(g);
}


} //~namespace ribi
} //~namespace cmap


#endif // CONCEPTMAPEDGEWRITER_H
