#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "conceptmapedge.h"

#include <sstream>

#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>

#include "conceptmap.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "conceptmapconceptfactory.h"
#include "conceptmaphelper.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "is_graphviz_friendly.h"
#include "xml.h"
#pragma GCC diagnostic pop

int ribi::cmap::Edge::sm_ids = 0; //!OCLINT must use static to track count

ribi::cmap::Edge::Edge(
  const Node& node,
  const bool has_head_arrow,
  const bool has_tail_arrow
) :
    m_has_head_arrow{has_head_arrow},
    m_has_tail_arrow{has_tail_arrow},
    m_id{sm_ids++},
    m_node{node}
{

}

ribi::cmap::Edge::~Edge() noexcept
{

}

void ribi::cmap::Edge::SetNode(const Node& node) noexcept
{
  m_node = node;
}

std::string ribi::cmap::Edge::ToStr() const noexcept
{

  std::stringstream s;
  s << (*this);
  return s.str();
}

std::string ribi::cmap::ToXml(
  const Edge& edge
) noexcept
{
  std::stringstream s;
  s << "<edge>";
  s << ToXml(edge.GetNode().GetConcept());
  s << "<has_head>" << edge.HasHeadArrow() << "</has_head>";
  s << "<has_tail>" << edge.HasTailArrow() << "</has_tail>";
  s << "<x>" << edge.GetNode().GetX() << "</x>";
  s << "<y>" << edge.GetNode().GetY() << "</y>";
  s << "</edge>";

  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,6) == "<edge>");
  assert(r.substr(r.size() - 7,7) == "</edge>");

  return r;
}

ribi::cmap::Edge ribi::cmap::XmlToEdge(
  const std::string& s
)
{
  using ribi::xml::StripXmlTag;
  assert(s.size() >= 13);
  assert(s.substr(0,6) == "<edge>");
  assert(s.substr(s.size() - 7,7) == "</edge>");
  //m_has_head
  bool has_head = false;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexHasHead());
    assert(v.size() == 1);
    has_head = boost::lexical_cast<bool>(StripXmlTag(v[0]));
  }
  //m_has_tail
  bool has_tail = false;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexHasTail());
    assert(v.size() == 1);
    has_tail = boost::lexical_cast<bool>(StripXmlTag(v[0]));
  }

  //m_x
  double x = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexX());
    assert(v.size() == 1);
    x = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  //m_y
  double y = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexY());
    assert(v.size() == 1);
    y = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  Node node(
    ExtractConceptFromXml(s),
    false, //is_center_node
    x,
    y
  );
  Edge edge(
    node, has_head, has_tail
  );
  return edge;
}

bool ribi::cmap::operator==(const ribi::cmap::Edge& lhs, const ribi::cmap::Edge& rhs)
{
  return lhs.GetNode() == rhs.GetNode()
    && lhs.HasHeadArrow() == rhs.HasHeadArrow()
    && lhs.HasTailArrow() == rhs.HasTailArrow()
  ;
  //Note: does not check for ID
}

bool ribi::cmap::operator!=(const cmap::Edge& lhs, const cmap::Edge& rhs)
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const cmap::Edge& lhs, const cmap::Edge& rhs)
{
  return lhs.GetNode() < rhs.GetNode();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Edge& edge) noexcept
{
  const std::string s{graphviz_encode(ToXml(edge))};
  assert(is_graphviz_friendly(s));
  os << s;
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Edge& edge)
{
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    assert(s != "0");
    if(s.size() > 7 && s.substr(s.size() - 7,7) == "</edge>") break;
  }
  edge = XmlToEdge(graphviz_decode(s));
  return is;
}
