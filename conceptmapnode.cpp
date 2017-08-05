#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapnode.h"

#include <cmath>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>

#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapnodefactory.h"
#include "is_graphviz_friendly.h"
#include "conceptmapcenternodefactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapregex.h"

#include "xml.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#pragma GCC diagnostic pop

int ribi::cmap::Node::sm_ids = 0; //!OCLINT use static to track instances

ribi::cmap::Node::Node(
  const Concept& concept,
  const bool is_center_node,
  const double x,
  const double y
) noexcept
  : m_concept{concept},
    m_id{sm_ids++},
    m_is_center_node{is_center_node},
    m_x(x),
    m_y(y)
{
  SetConcept(concept);
  assert(m_concept == concept);
}

int ribi::cmap::CountCenterNodes(const std::vector<Node>& nodes) noexcept
{
  return container().CountIf(nodes,
    [](const Node& node)
    {
      return IsCenterNode(node);
    }
  );
}

bool ribi::cmap::ExtractIsCenterNodeFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexIsCenterNode());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one center node tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
}

double ribi::cmap::ExtractXfromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexX());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one X tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
}

double ribi::cmap::ExtractYfromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexY());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one Y tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
}

/*
std::vector<ribi::cmap::Node>::const_iterator
ribi::cmap::FindCenterNode(const std::vector<Node>& nodes) noexcept
{
  assert(!"Am I used?");
  return std::find_if(
    std::begin(nodes),std::end(nodes),
    [](const Node& node) {
      return IsCenterNode(node);
    }
  );
}
*/

std::vector<ribi::cmap::Node> ribi::cmap::Node::GetTests() noexcept
{
  const auto test_concepts = ConceptFactory().GetTests();
  std::vector<Node> result;
  std::for_each(test_concepts.begin(),test_concepts.end(),
    [&result](const Concept& concept)
    {
      const int x = (std::rand() % 256) - 128;
      const int y = (std::rand() % 256) - 128;
      Node node(concept,x,y);
      result.push_back(node);
    }
  );
  return result;
}

std::string ribi::cmap::GetText(const Node& node) noexcept
{
  return node.GetName();
}

bool ribi::cmap::HasExamples(const Node& node) noexcept
{
  return HasExamples(node.GetConcept());
}

bool ribi::cmap::HasSameContent(const Node& lhs, const Node& rhs) noexcept
{
  return lhs.GetConcept() == rhs.GetConcept();
}

/*
bool ribi::cmap::HaveSameIds(const Node& lhs, const Node& rhs) noexcept
{
  return lhs.m_id == rhs.m_id;
}
*/

bool ribi::cmap::IsCenterNode(const Node& node) noexcept
{
  return node.IsCenterNode();
}

bool ribi::cmap::NodeHasExamples(const Node& node) noexcept
{
  return HasExamples(node.GetConcept());
}

void ribi::cmap::Node::SetConcept(const Concept& concept) noexcept
{
  m_concept = concept;
}

void ribi::cmap::Node::SetX(const double x) noexcept
{
  m_x = x;
}

void ribi::cmap::Node::SetY(const double y) noexcept
{
  m_y = y;
}


std::string ribi::cmap::Node::ToStr() const noexcept
{
  std::stringstream s;
  s << *this;
  return s.str();
}

std::string ribi::cmap::ToXml(const Node& node) noexcept
{
  std::stringstream s;
  s << "<node>";
  s << ToXml(node.GetConcept());
  s << "<x>" << node.GetX() << "</x>";
  s << "<y>" << node.GetY() << "</y>";
  s << "<is_center_node>" << node.IsCenterNode() << "</is_center_node>";
  s << "</node>";
  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,6) == "<node>");
  assert(r.substr(r.size() - 7,7) == "</node>");

  return r;
}

ribi::cmap::Node ribi::cmap::XmlToNode(const std::string& s)
{
  if (s.size() < 13)
  {
    std::stringstream msg;
    msg << __func__ << ": string too short";
    throw std::invalid_argument(msg.str());
  }
  if (s.substr(0,6) != "<node>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect starting tag";
    throw std::invalid_argument(msg.str());
  }
  if (s.substr(s.size() - 7,7) != "</node>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect ending tag";
    throw std::invalid_argument(msg.str());
  }
  return Node(
    ExtractConceptFromXml(s),
    ExtractIsCenterNodeFromXml(s),
    ExtractXfromXml(s),
    ExtractYfromXml(s)
  );
}

bool ribi::cmap::operator==(const Node& lhs, const Node& rhs) noexcept
{
  const double e{0.001};
  if (lhs.GetConcept() != rhs.GetConcept())
  {
    return false;
  }
  if (std::abs(lhs.GetX() - rhs.GetX()) > e)
  {
    return false;
  }
  if (std::abs(lhs.GetY() - rhs.GetY()) > e)
  {
    return false;
  }
  return true;
}

bool ribi::cmap::operator!=(const Node& lhs, const Node& rhs) noexcept
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const Node& lhs, const Node& rhs) noexcept
{
  if (lhs.GetX() < rhs.GetX()) return true;
  if (lhs.GetX() > rhs.GetX()) return false;
  if (lhs.GetY() < rhs.GetY()) return true;
  if (lhs.GetY() > rhs.GetY()) return false;
  return lhs.GetConcept() < rhs.GetConcept();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Node& node) noexcept
{
  const std::string s{graphviz_encode(ToXml(node))};
  assert(is_graphviz_friendly(s));
  os << s;
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Node& node)
{
  is >> std::noskipws;
  std::string s;
  while (1) //Eat whitespace
  {
    char c;
    is >> c;
    assert(c != '\0');
    if (c == ' ') continue;
    s += c;
    break; //!OCLINT No idea how to do this more elegantly
  }

  while (1)
  {
    char c;
    is >> c;
    s += c;
    assert(s.size() < 6 || s.substr(0,6) == "<node>");
    if(s.size() > 7 && s.substr(s.size() - 7,7) == "</node>") break;
  }

  node = XmlToNode(graphviz_decode(s));
  return is;
}

