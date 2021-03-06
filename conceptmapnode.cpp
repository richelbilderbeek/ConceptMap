#include "conceptmapnode.h"

#include <cassert>
#include <sstream>

#include "conceptmapconceptfactory.h"
#include "conceptmapregex.h"
#include "container.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "is_graphviz_friendly.h"
#include "xml.h"

int ribi::cmap::Node::sm_ids = 0; //!OCLINT use static to track instances

ribi::cmap::Node::Node(
  const Concept& concept,
  const NodeType type,
  const double x,
  const double y,
  const int id
) noexcept
  : m_concept{concept},
    m_id{AssignId(id)},
    m_type{type},
    m_x(x),
    m_y(y)
{
  SetConcept(concept);
  assert(m_concept == concept);
}

int ribi::cmap::Node::AssignId(const int id) //!OCLINT
{
  if (id < 0)
  {
    const int new_id{sm_ids};
    ++sm_ids;
    return new_id;
  }
  return id;
}

std::vector<std::string> ribi::cmap::CollectExamplesTexts(const Node& node) noexcept
{
  return CollectExamplesTexts(node.GetConcept());
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

int ribi::cmap::CountExamples(const Node& node) noexcept
{
  return CountExamples(node.GetConcept());
}

ribi::cmap::NodeType ribi::cmap::ExtractIsCenterNodeFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexIsCenterNode());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one center node tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return ToNodeType(ribi::xml::StripXmlTag(v[0]));
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

const ribi::cmap::Concept& ribi::cmap::GetConcept(const Node& node) noexcept
{
  return node.GetConcept();
}

const ribi::cmap::Example& ribi::cmap::GetExample(
  const Node& node,
  const int i)
{
  assert(i >= 0);
  assert(i < CountExamples(node));
  return GetExamples(node)[i];
}

ribi::cmap::Example& ribi::cmap::GetExample(
  Node& node,
  const int i)
{
  assert(i >= 0);
  assert(i < CountExamples(node));
  return GetExamples(node)[i];
}

const ribi::cmap::Examples& ribi::cmap::GetExamples(const Node& node) noexcept
{
  return GetExamples(node.GetConcept());
}

ribi::cmap::Examples& ribi::cmap::GetExamples(Node& node) noexcept
{
  return GetExamples(node.GetConcept());
}

std::vector<ribi::cmap::Node> ribi::cmap::Node::GetTests() noexcept
{
  const auto test_concepts = ConceptFactory().GetTests();
  std::vector<Node> result;
  std::for_each(test_concepts.begin(),test_concepts.end(),
    [&result](const Concept& concept)
    {
      const int x = (std::rand() % 256) - 128;
      const int y = (std::rand() % 256) - 128;
      const Node node(concept, NodeType::normal, x, y);
      result.push_back(node);
    }
  );
  return result;
}

bool ribi::cmap::GetIsComplex(const Node& node) noexcept
{
  return GetIsComplex(node.GetConcept());
}

int ribi::cmap::GetRatingComplexity(const Node& node) noexcept
{
  return GetRatingComplexity(GetConcept(node));
}

int ribi::cmap::GetRatingConcreteness(const Node& node) noexcept
{
  return GetRatingConcreteness(GetConcept(node));
}

int ribi::cmap::GetRatingSpecificity(const Node& node) noexcept
{
  return GetRatingSpecificity(GetConcept(node));
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

bool ribi::cmap::HasSameData(const Node& lhs, const Node& rhs) noexcept
{
  return HasSameContent(lhs, rhs)
    && lhs.GetX() == rhs.GetX()
    && lhs.GetY() == rhs.GetY()
  ;
}

bool ribi::cmap::HasSimilarData(
  const Node& lhs,
  const Node& rhs,
  const double tolerance) noexcept
{
  return HasSameContent(lhs, rhs)
    && std::abs(lhs.GetX() - rhs.GetX()) < tolerance
    && std::abs(lhs.GetY() - rhs.GetY()) < tolerance
  ;
}

bool ribi::cmap::HasSimilarData(
  const std::vector<Node>& lhs,
  const std::vector<Node>& rhs,
  const double tolerance)
{
  if (lhs.size() != rhs.size())
  {
    throw std::invalid_argument("lhs and rhs must have equal number of Nodes");
  }
  const int sz = lhs.size();
  for (int i=0; i!=sz; ++i)
  {
    if (!HasSimilarData(lhs[i], rhs[i], tolerance)) return false;
  }
  return true;
}



bool ribi::cmap::IsCenterNode(const Node& node) noexcept
{
  return node.GetType() == NodeType::center;
}

void ribi::cmap::Move(Node& node, const double dx, const double dy)
{
  node.SetX(node.GetX() + dx);
  node.SetY(node.GetY() + dy);
}

bool ribi::cmap::NodeHasExamples(const Node& node) noexcept
{
  return HasExamples(node.GetConcept());
}

void ribi::cmap::Node::SetConcept(const Concept& concept) noexcept
{
  m_concept = concept;
}

void ribi::cmap::SetConcept(Node& node, const Concept& concept)
{
  node.SetConcept(concept);
}

void ribi::cmap::SetExamples(Node& node, const Examples& examples) noexcept
{
  SetExamples(node.GetConcept(), examples);
}

void ribi::cmap::SetIsComplex(Node& node, const bool is_complex)
{
  SetIsComplex(node.GetConcept(), is_complex);
}

void ribi::cmap::SetRatingComplexity(
  Node& node,
  const int rating_complexity
)
{
  SetRatingComplexity(node.GetConcept(), rating_complexity);
}

void ribi::cmap::SetRatingConcreteness(
  Node& node,
  const int rating_concreteness
)
{
  SetRatingConcreteness(node.GetConcept(), rating_concreteness);
}

void ribi::cmap::SetRatingSpecificity(
  Node& node,
  const int rating_specificity
)
{
  SetRatingSpecificity(node.GetConcept(), rating_specificity);
}

void ribi::cmap::SetText(Node& node, const std::string& text)
{
  SetText(node.GetConcept(), text);
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
  s << "<type>" << node.GetType() << "</type>";
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
  //Each Node has a unique ID
  if (lhs.GetId() != rhs.GetId())
  {
    return false;
  }
  assert(HasSameData(lhs, rhs));
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

