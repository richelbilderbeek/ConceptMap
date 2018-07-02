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
      << "\"" //Do not add comma here, as this may be the last item
    ;
    //Stopped saving selectedness to file
    //if (get(m_is_selected_map, ed))
    //{
    //  out << ", style = \"dashed\""; //Do not add comma here, as this may be the last item
    //}

    const bool has_head{edge.HasHeadArrow()};
    const bool has_tail{edge.HasTailArrow()};
    if ( has_head &&  has_tail)
    {
      out << ", dir = \"both\", arrowhead = \"normal\", arrowtail = \"normal\"";
    }
    else if ( has_head && !has_tail)
    {
      out << ", dir = \"forward\", arrowhead = \"normal\"";
    }
    else if (!has_head && has_tail)
    {
      out << ", dir = \"back\", arrowtail = \"normal\"";
    }
    else
    {
      assert(!has_head && !has_tail);
      //No need to add something
    }
    out << "]";
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
