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

std::vector<std::string> ribi::cmap::CollectExamplesTexts(const Edge& edge) noexcept
{
  return CollectExamplesTexts(edge.GetNode());
}


int ribi::cmap::CountExamples(const Edge& edge) noexcept
{
  return CountExamples(edge.GetNode());
}

const ribi::cmap::Concept& ribi::cmap::GetConcept(const Edge& edge) noexcept
{
  return GetConcept(edge.GetNode());
}

std::string ribi::cmap::GetText(const Edge& edge) noexcept
{
  return GetText(edge.GetNode());
}

double ribi::cmap::GetX(const Edge& edge) noexcept
{
  return GetX(edge.GetNode());
}

double ribi::cmap::GetY(const Edge& edge) noexcept
{
  return GetY(edge.GetNode());
}

bool ribi::cmap::HasExamples(const Edge& e) noexcept
{
  return HasExamples(e.GetNode());
}

bool ribi::cmap::HasSameData(const Edge& lhs, const Edge& rhs) noexcept
{
  return HasSameData(lhs.GetNode(), rhs.GetNode())
    && lhs.HasHeadArrow() == rhs.HasHeadArrow()
    && lhs.HasTailArrow() == rhs.HasTailArrow()
  ;
}

bool ribi::cmap::HasSameData(const std::vector<Edge>& lhs, const std::vector<Edge>& rhs)
{
  if (lhs.size() != rhs.size())
  {
    throw std::invalid_argument("lhs and rhs must have equal number of Edges");
  }
  const int sz = lhs.size();
  for (int i=0; i!=sz; ++i)
  {
    if (!HasSameData(lhs[i], rhs[i])) return false;
  }
  return true;
}

bool ribi::cmap::HasSimilarData(const Edge& lhs, const Edge& rhs, const double tolerance) noexcept
{
  return HasSimilarData(lhs.GetNode(), rhs.GetNode(), tolerance)
    && lhs.HasHeadArrow() == rhs.HasHeadArrow()
    && lhs.HasTailArrow() == rhs.HasTailArrow()
  ;
}

bool ribi::cmap::HasSimilarData(
  const std::vector<Edge>& lhs,
  const std::vector<Edge>& rhs,
  const double tolerance)
{
  if (lhs.size() != rhs.size())
  {
    throw std::invalid_argument("lhs and rhs must have equal number of Edges");
  }
  const int sz = lhs.size();
  for (int i=0; i!=sz; ++i)
  {
    if (!HasSimilarData(lhs[i], rhs[i], tolerance)) return false;
  }
  return true;

}

void ribi::cmap::Move(Edge& edge, const double dx, const double dy)
{
  Move(edge.GetNode(), dx, dy);
}

void ribi::cmap::SetConcept(Edge& edge, const Concept& concept) noexcept
{
  SetConcept(edge.GetNode(), concept);
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
  s << "<x>" << GetX(edge) << "</x>";
  s << "<y>" << GetY(edge) << "</y>";
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
  //Node at middle of edge is never a center immutable node
  Node node(
    ExtractConceptFromXml(s),
    NodeType::normal,
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
  if (lhs.GetId() != rhs.GetId()) return false;
  assert(HasSameData(lhs, rhs));
  return true;
}

bool ribi::cmap::operator!=(const Edge& lhs, const Edge& rhs)
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const Edge& lhs, const Edge& rhs)
{
  if (lhs.HasHeadArrow() < rhs.HasHeadArrow()) return true;
  if (lhs.HasHeadArrow() > rhs.HasHeadArrow()) return false;
  if (lhs.HasTailArrow() < rhs.HasTailArrow()) return true;
  if (lhs.HasTailArrow() > rhs.HasTailArrow()) return false;
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
