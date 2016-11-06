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
    true, //is_center_node
    0.0, //x
    0.0  //y
  );
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
    true, //is_center_node
    x,
    y
  );
  assert(concept == node.GetConcept());
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.IsCenterNode());
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
    true,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.IsCenterNode());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetNasty0() const noexcept
{
  auto node = NodeFactory().GetNasty0();
  node.SetIsCenterNode(true);
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
  for (auto& node: v) { node.SetIsCenterNode(true); }
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::CenterNodeFactory::GetNastyTests() const noexcept
{
  auto v = NodeFactory().GetNastyTests();
  for (auto& node: v) { node.SetIsCenterNode(true); }
  return v;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetTest(test_index);
  node.SetIsCenterNode(true);
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetNastyTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetNastyTest(test_index);
  node.SetIsCenterNode(true);
  return node;
}

