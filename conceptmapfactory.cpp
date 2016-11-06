#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>

#include "conceptmapfactory.h"

#include <cassert>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>

#include "add_custom_and_selectable_edge_between_vertices.h"
#include "conceptmapcenternodefactory.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapexamplefactory.h"
#include "add_custom_and_selectable_vertex.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "set_my_custom_vertexes.h"

#pragma GCC diagnostic pop

ribi::cmap::ConceptMapFactory::ConceptMapFactory() noexcept
{

}

ribi::cmap::VertexDescriptor ribi::cmap::AddVertex(
  const Node& node, ConceptMap& g
) noexcept
{
  return add_custom_and_selectable_vertex(node, false, g);
}

void ribi::cmap::AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  ConceptMap& g
) noexcept
{
  add_custom_and_selectable_edge_between_vertices(edge, false, vd_from, vd_to, g);
}

std::vector<ribi::cmap::ConceptMap>
ribi::cmap::ConceptMapFactory::GetNastyTests() const noexcept
{
  std::vector<ConceptMap> v{
    GetNasty0()
  };
  return v;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get0() const noexcept
{
  return ConceptMap();
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get1() const noexcept
{
  ConceptMap g;
  AddVertex(CenterNodeFactory().GetTest(0),g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get2() const noexcept
{
  ConceptMap g;
  AddVertex(CenterNodeFactory().GetTest(1),g);
  AddVertex(NodeFactory().GetTest(0), g);
  AddVertex(NodeFactory().GetTest(1), g);
  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get3() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetTest(2),g);
  const auto vd_2 = AddVertex(NodeFactory().GetTest(2), g);

  AddEdge(
    EdgeFactory().GetTest(1),vd_1,vd_2,g
  );
  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get4() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(0), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  AddEdge(
    EdgeFactory().GetTest(2),
    vd_1,vd_2,g
  );
  AddEdge(
    EdgeFactory().GetTest(2),
    vd_2,vd_3,g
  );
  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get5() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(1), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetNastyTest(0),vd_1,vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(1),vd_2,vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(2),vd_3,vd_1,g);
  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get6() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(2), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetTest(2),vd_1,vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(0),vd_2,vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(1),vd_3,vd_1,g);
  AddEdge(EdgeFactory().GetNastyTest(2),vd_4,vd_1,g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get7() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty2(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty1(), g);

  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_1, vd_2, g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get8() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty2(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty0(), g);

  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_1, vd_2, g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get9() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty2(), g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_1, vd_2, g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get10() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings(
    "X: This is the center node concept that can have no examples, "
    "otherwise each of its example name would be multiple lines"),
    g
  );
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings(
    "A: This is a concept that has none of all types of competencies "
    "as its examples, oterwise each of its example name "
    "would be multiple lines"),
    g
  );
  const auto vd_3 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty2(), g);

  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_1, vd_2, g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get11() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty2(), g);

  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_1, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty2(),false,true), vd_1, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty0(),false,true), vd_1, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty1(),false,true), vd_1, vd_5, g);

  return RepositionNodes(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetNasty0() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(0), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetNastyTest(0),vd_1,vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(1),vd_2,vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(2),vd_3,vd_1,g);
  AddEdge(EdgeFactory().GetNastyTest(0),vd_4,vd_1,g);
  return g;
}

ribi::cmap::ConceptMap
ribi::cmap::ConceptMapFactory::GetRateConceptTallyDialogExample() const noexcept
{
  ConceptMap g;
  const Examples examples_1(
    {
      Example("Always establish order first"),
      Example("Punishment"),
    }
  );
  const Examples examples_2(
    {
      Example("Students teaching each other, get to know each other")
    }
  );

  const Concept concept1("Learning environment");
  const Concept concept2("Order");
  const Concept concept3("Social safety");
  const auto vd_1 = AddVertex(Node(concept1), g);
  const auto vd_2 = AddVertex(Node(concept2), g);
  const auto vd_3 = AddVertex(Node(concept3), g);
  AddEdge(Edge(Node(Concept("prerequisite", examples_1)),true),vd_2,vd_1,g);
  AddEdge(Edge(Node(Concept("strengthen", examples_2)),true,true),vd_2,vd_3,g);
  return g;
}

ribi::cmap::ConceptMap
ribi::cmap::ConceptMapFactory::GetQtRatedConceptDialogExample() const noexcept
{
  ConceptMap g;
  const Examples examples_node_1(
    {
      Example("Arduino course"),
      Example("Dojo")
    }
  );

  const Examples examples_edge_1(
    {
      Example("Always establish order first"),
      Example("Punishment"),
    }
  );
  const Examples examples_edge_2(
    {
      Example("Students teaching each other, get to know each other")
    }
  );

  const Concept concept1("Learning environment", examples_node_1);
  const Concept concept2("Order");
  const Concept concept3("Social safety");
  const auto vd_1 = AddVertex(Node(concept1), g);
  const auto vd_2 = AddVertex(Node(concept2), g);
  const auto vd_3 = AddVertex(Node(concept3), g);
  AddEdge(Edge(Node(Concept("prerequisite", examples_edge_1)),true),vd_2,vd_1,g);
  AddEdge(Edge(Node(Concept("strengthen", examples_edge_2)),true,true),vd_2,vd_3,g);
  return g;
}

std::vector<ribi::cmap::ConceptMap >
ribi::cmap::ConceptMapFactory::GetAllTests() const noexcept
{
  std::vector<ConceptMap> v{
    Get0(),
    Get1(),
    Get2(),
    Get3(),
    Get4(),
    Get5(),
    Get6(),
    Get7(),
    Get8(),
    Get9(),
    Get10(),
    Get11()
  };
  return v;
}

ribi::cmap::ConceptMap ribi::cmap::RepositionNodes(ConceptMap& g)
{
  auto nodes = GetNodes(g);
  const int n_center{CountCenterNodes(nodes)};
  const int n_normal{static_cast<int>(nodes.size()) - n_center};


  double delta_angle{
    boost::math::constants::pi<double>() / static_cast<double>(n_normal)
  };
  double angle{0.0};
  for (Node& node: nodes)
  {
    if (IsCenterNode(node))
    {
      node.SetPos(0.0, 0.0);
    }
    else
    {
      const double r{200.0}; //ray, half of diameter
      const double x{ std::sin(angle) * r};
      const double y{-std::cos(angle) * r};
      node.SetPos(x, y);
      angle += delta_angle;
    }
  }

  ::set_my_custom_vertexes(g, nodes);
  return g;
}
