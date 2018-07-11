

#include "conceptmapnodefactory.h"


#include "conceptmapcenternodefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapregex.h"
#include "container.h"

#include "xml.h"


ribi::cmap::NodeFactory::NodeFactory()
{

}

ribi::cmap::Node ribi::cmap::NodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  Node node(
    ConceptFactory().Create(name,examples),
    NodeType::normal,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

int ribi::cmap::NodeFactory::GetNumberOfTests() const noexcept
{
  return static_cast<int>(GetTests().size());
}

int ribi::cmap::NodeFactory::GetNumberOfNastyTests() const noexcept
{
  return static_cast<int>(GetNastyTests().size());
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetTest(const int i) const
{
  const auto tests = GetTests();
  return tests.at(i);
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty0() const noexcept
{
  return Node(
    ConceptFactory().GetNasty0(),
    NodeType::normal,
    1.2,
    3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty1() const noexcept
{
  return Node(
    ConceptFactory().GetNasty1(),
    NodeType::normal,
    -1.2,
    3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty2() const noexcept
{
  return Node(
    ConceptFactory().GetNasty2(),
    NodeType::normal,
    -1.2,
    -3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNastyTest(const int i) const noexcept
{
  const auto tests = GetNastyTests();
  assert(i >= 0);
  assert(i < static_cast<int>(tests.size()));
  return tests[i];
}

std::vector<ribi::cmap::Node> ribi::cmap::NodeFactory::GetNastyTests() const noexcept
{
  std::vector<Node> nodes;
  const auto v = ConceptFactory().GetNastyTests();
  std::transform(v.begin(),v.end(),std::back_inserter(nodes),
    [](const Concept& c)
    {
      const double x{1.2};
      const double y{3.4};
      const Node p{c, NodeType::normal, x, y};
      return p;
    }
  );
  return nodes;
}

std::vector<ribi::cmap::Node> ribi::cmap::NodeFactory::GetTests() const noexcept
{
  const auto v = ConceptFactory().GetTests();
  std::vector<Node> nodes;
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(nodes),
    [](const Concept& c)
    {
      const double x{1.2};
      const double y{3.4};
      const Node p{c, NodeType::normal, x, y};
      return p;
    }
  );
  return nodes;
}

