#include "conceptmapcenternodefactory.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>


#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"

ribi::cmap::CenterNodeFactory::CenterNodeFactory()
{
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::Create() const noexcept
{
  Node node(
    ConceptFactory().Create(),
    NodeType::center,
    0.0, //x
    0.0  //y
  );
  assert(node.GetConcept().GetExamples().Get().empty());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::Create(
  const Concept& concept,
  const double x,
  const double y
) const noexcept
{
  Node node(
    concept,
    NodeType::center,
    x,
    y
  );
  assert(concept == node.GetConcept());
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.GetType() == NodeType::center);
  assert(node.GetConcept().GetExamples().Get().empty());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  Node node(
    ConceptFactory().Create(name,examples),
    NodeType::center,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.GetType() == NodeType::center);
  assert(node.GetConcept().GetExamples().Get().empty());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetNasty0() const noexcept
{
  auto node = NodeFactory().GetNasty0();
  node.SetType(NodeType::center);
  assert(node.GetConcept().GetExamples().Get().empty());
  return node;
}

int ribi::cmap::CenterNodeFactory::GetNumberOfTests() const noexcept
{
  return NodeFactory().GetNumberOfTests();
}

int ribi::cmap::CenterNodeFactory::GetNumberOfNastyTests() const noexcept
{
  return NodeFactory().GetNumberOfNastyTests();
}

std::vector<ribi::cmap::Node> ribi::cmap::CenterNodeFactory::GetTests() const noexcept
{
  auto v = NodeFactory().GetTests();
  for (Node& node: v)
  {
    node.SetType(NodeType::center);
    //A center node does not have examples
    node.GetConcept().GetExamples().Get().clear();
  }
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::CenterNodeFactory::GetNastyTests() const noexcept
{
  auto v = NodeFactory().GetNastyTests();
  for (Node& node: v)
  {
    node.SetType(NodeType::center);

    //A center node does not have examples
    node.GetConcept().GetExamples().Get().clear();
  }
  return v;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetTest(test_index);
  node.SetType(NodeType::center);

  //A center node does not have examples
  node.GetConcept().GetExamples().Get().clear();

  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetNastyTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetNastyTest(test_index);
  node.SetType(NodeType::center);

  //A center node does not have examples
  node.GetConcept().GetExamples().Get().clear();
  return node;
}

