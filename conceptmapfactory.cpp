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

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetStarShaped() const noexcept
{
  ConceptMap g;

  //Not needed to connect to center node
  AddVertex(CenterNodeFactory().CreateFromStrings(
    "A (not truly) star-shaped concept map is ..."), g
  );
  const auto vd_1 = AddVertex(NodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("Useful"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("In"), g);
  const auto vd_4 = AddVertex(NodeFactory().CreateFromStrings("Debugging"), g);

  AddEdge(Edge(NodeFactory().CreateFromStrings("2"),false,true), vd_1, vd_2, g);
  AddEdge(Edge(NodeFactory().CreateFromStrings("3"),false,true), vd_1, vd_3, g);
  AddEdge(Edge(NodeFactory().CreateFromStrings("4"),false,true), vd_1, vd_4, g);

  return RepositionNodes(g);

}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetThreeNodeTwoEdge() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(Node(Concept("center"), true, 100, 200), g);
  const auto vd_2 = AddVertex(Node(Concept("one"), false, 300, 250), g);
  const auto vd_3 = AddVertex(Node(Concept("two"), false, 500, 350), g);
  AddEdge(Edge(Node(Concept( "first"), false, 150, 225)), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("second"), false, 350, 275)), vd_2, vd_3, g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetThreeNodeTwoEdgeNoCenter() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(
    Node(Concept("one", Examples( { Example("unos"), Example("een") } )),
    false, 100, 200), g);
  const auto vd_2 = AddVertex(
    Node(Concept("two", Examples( { Example("Zwei"), Example("twee") } )),
    false, 300, 250), g);
  const auto vd_3 = AddVertex(
    Node(Concept("three", Examples( { Example("drie"), Example("Drei") } )),
    false, 500, 350), g);
  AddEdge(Edge(Node(Concept("first",
    Examples( { Example("erster"), Example("premier") } )),
    false, 150, 225)), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("second",
    Examples( { Example("zweite"), Example("deuxieme") } )),
    false, 350, 275)), vd_2, vd_3, g);
  return g;
}


ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetTwoNodeOneEdge() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(Node(Concept("center"), true, 100, 200), g);
  const auto vd_2 = AddVertex(Node(Concept("one",
    Examples( { Example("unos"), Example("een") } )), false, 300, 250), g);
  AddEdge(Edge(Node(Concept( "first"), false, 150, 225)), vd_1, vd_2, g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetTwoNodeOneEdgeNoCenter() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_2 = AddVertex(Node(Concept("one"), false, 300, 250), g);
  const auto vd_3 = AddVertex(Node(Concept("two"), false, 500, 350), g);
  AddEdge(Edge(Node(Concept("second",
    Examples( { Example("zweite"), Example("deuxieme") } )), false, 350, 275)), vd_2, vd_3, g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetWithExamplesWithCompetencies(
  const std::vector<Competency>& competencies
) const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("A center node is ..."), g);
  Node n = NodeFactory().CreateFromStrings("not me");
  std::vector<Example> examples;
  {
    int i = 0;
    for (const auto competency: competencies)
    {
      const std::string text = "Example " + std::to_string(i);
      examples.push_back(
        Example(text, competency)
      );
      ++i;
    }
  }
  n.GetConcept().SetExamples(Examples(examples));
  const auto vd_2 = AddVertex(n, g);

  AddEdge(Edge(NodeFactory().GetTest(0),false,false), vd_1, vd_2, g);

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
  return RepositionNodes(g);
  //return g;
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
  return RepositionNodes(g);
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
