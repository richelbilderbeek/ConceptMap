#include "conceptmapedgefactory.h"

#include <cassert>

#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"

ribi::cmap::EdgeFactory::EdgeFactory() noexcept
{

}


ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);

  const double x{(from.GetX() + to.GetX()) / 2.0};
  const double y{(from.GetY() + to.GetY()) / 2.0};
  const auto concept = ConceptFactory().Create();
  //Node at the middle of an edge is a normal node
  const Node node(concept, NodeType::normal, x, y);
  Edge p(node);
  return p;
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& node
) const noexcept
{
  Edge p(node);
  return p;
}

int ribi::cmap::EdgeFactory::GetNumberOfTests() const noexcept
{
  return ConceptFactory().GetNumberOfTests();
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::GetTest(
  const int index
) const noexcept
{
  //Nodes may be similar, but not the same
  const auto v = GetTests();
  assert(index >= 0);
  assert(index < static_cast<int>(v.size()));
  return v[index];
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::GetNastyTest(
  const int index
) const noexcept
{
  const auto v = GetNastyTests();
  assert(index >= 0);
  assert(index < static_cast<int>(v.size()));
  return v[index];
}

std::vector<ribi::cmap::Edge> ribi::cmap::EdgeFactory::GetNastyTests() const noexcept
{
  std::vector<Edge> result;

  for (const auto node: NodeFactory().GetNastyTests())
  {
    Edge edge(node);
    edge.SetHeadArrow(true);
    result.emplace_back(edge);
  }
  return result;
}


std::vector<ribi::cmap::Edge> ribi::cmap::EdgeFactory::GetTests() const noexcept
{
  const int n{NodeFactory().GetNumberOfTests()};
  std::vector<Edge> result;

  for(int i{0}; i!=n; ++i)
  {
    {
      const auto node = NodeFactory().GetTest(i);
      Edge edge(node);
      edge.SetHeadArrow(true);
      result.emplace_back(edge);
    }
  }
  assert(GetNumberOfTests() == static_cast<int>(result.size()));
  return result;
}


