#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>

#include "conceptmapfactory.h"

#include <cassert>
#include <stdexcept>
#include <boost/math/constants/constants.hpp>

#include "add_bundled_edge_between_vertices.h"
#include "conceptmapcenternodefactory.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapexamplefactory.h"
#include "add_bundled_vertex.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "set_my_bundled_vertexes.h"

ribi::cmap::ConceptMapFactory::ConceptMapFactory() noexcept
{

}

ribi::cmap::VertexDescriptor ribi::cmap::AddVertex(
  const Node& node, ConceptMap& g
) noexcept
{
  return add_bundled_vertex(node, g);
}

void ribi::cmap::AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  ConceptMap& g
) noexcept
{
  add_bundled_edge_between_vertices(edge, vd_from, vd_to, g);
}

std::vector<ribi::cmap::ConceptMap>
ribi::cmap::ConceptMapFactory::GetNastyTests() const noexcept
{
  std::vector<ConceptMap> v{
    GetNasty0()
  };
  return v;
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
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get3() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetTest(2),g);
  const auto vd_2 = AddVertex(NodeFactory().GetTest(2), g);

  AddEdge(
    EdgeFactory().GetTest(1), vd_1, vd_2,g
  );
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get4() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(0), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  AddEdge(
    EdgeFactory().GetTest(2),
    vd_1, vd_2,g
  );
  AddEdge(
    EdgeFactory().GetTest(2),
    vd_2, vd_3,g
  );
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get5() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(1), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetNastyTest(0), vd_1, vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(1), vd_2, vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(2), vd_3, vd_1,g);
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get6() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(2), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetTest(2), vd_1, vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(0), vd_2, vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(1), vd_3, vd_1,g);
  AddEdge(EdgeFactory().GetNastyTest(2), vd_4, vd_1,g);
  assert(CountCenterNodes(g) == 1);
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get7() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty2(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty1(), g);

  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_1, vd_2, g);

  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get8() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty2(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty0(), g);

  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_1, vd_2, g);

  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get9() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty2(), g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_1, vd_2, g);

  return Reposition(g);
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

  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_1, vd_2, g);

  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Get11() const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNasty0(), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNasty1(), g);
  const auto vd_5 = AddVertex(NodeFactory().GetNasty2(), g);

  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_3, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_4, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_5, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_2, vd_5, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_1, vd_2, g);
  AddEdge(Edge(NodeFactory().GetNasty2(), false, true), vd_1, vd_3, g);
  AddEdge(Edge(NodeFactory().GetNasty0(), false, true), vd_1, vd_4, g);
  AddEdge(Edge(NodeFactory().GetNasty1(), false, true), vd_1, vd_5, g);

  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetAllArrowTypes() const noexcept
{
  ConceptMap g;
  // center --> A <-> B <-- C --- D
  const auto vd_1 = AddVertex(Node(Concept("center"), NodeType::center), g);
  const auto vd_2 = AddVertex(Node(Concept("A")), g);
  const auto vd_3 = AddVertex(Node(Concept("B")), g);
  const auto vd_4 = AddVertex(Node(Concept("C")), g);
  const auto vd_5 = AddVertex(Node(Concept("D")), g);
  AddEdge(Edge(Node(Concept("-->")), true , false), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("<->")), true , true ), vd_2, vd_3, g);
  AddEdge(Edge(Node(Concept("<--")), false, true ), vd_3, vd_4, g);
  AddEdge(Edge(Node(Concept("---")), false, false), vd_4, vd_5, g);
  return Reposition(g);

}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetLevel() const noexcept
{
  ConceptMap g;
  // Level:
  //      2 <- 1 <- 0 -> 1 -> 2
  //                |    |
  //                v    v
  //                1    2
  // Vertex:
  //      3 <- 2 <- 1 -> 5 -> 6
  //                |    |
  //                v    v
  //                8    9

  const auto vd_1 = boost::add_vertex(Node(Concept("0"), NodeType::center), g);
  //Messed up order a bit on purpose: this concept map is used to demonstrate
  //GetNodesSortedByLevel
  const auto vd_2 = boost::add_vertex(Node(Concept("1"), NodeType::center), g);
  const auto vd_3 = boost::add_vertex(Node(Concept("2"), NodeType::center), g);
  const auto vd_5 = boost::add_vertex(Node(Concept("1"), NodeType::center), g);
  const auto vd_6 = boost::add_vertex(Node(Concept("2"), NodeType::center), g);
  const auto vd_8 = boost::add_vertex(Node(Concept("1"), NodeType::center), g);
  const auto vd_9 = boost::add_vertex(Node(Concept("2"), NodeType::center), g);
  boost::add_edge(vd_1, vd_2, Edge(), g);
  boost::add_edge(vd_2, vd_3, Edge(), g);
  boost::add_edge(vd_1, vd_5, Edge(), g);
  boost::add_edge(vd_5, vd_6, Edge(), g);
  boost::add_edge(vd_1, vd_8, Edge(), g);
  boost::add_edge(vd_5, vd_9, Edge(), g);
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetLonelyQtCenterNode() const noexcept
{
  ConceptMap g;
  AddVertex(
    Node(Concept("center"),
    NodeType::center, 100, 200), g);
  return g;
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

  AddEdge(Edge(NodeFactory().CreateFromStrings("2"), false, true), vd_1, vd_2, g);
  AddEdge(Edge(NodeFactory().CreateFromStrings("3"), false, true), vd_1, vd_3, g);
  AddEdge(Edge(NodeFactory().CreateFromStrings("4"), false, true), vd_1, vd_4, g);

  return Reposition(g);

}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetThreeNodeTwoEdge() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(Node(Concept("center"), NodeType::center, 100, 200), g);
  const auto vd_2 = AddVertex(Node(Concept("one"), NodeType::normal, 300, 250), g);
  const auto vd_3 = AddVertex(Node(Concept("two"), NodeType::normal, 500, 350), g);

  AddEdge(Edge(Node(Concept("first",
    Examples( { Example("erster"), Example("premier") } )),
    NodeType::normal, 150, 225)), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("second",
    Examples( { Example("zweite"), Example("deuxieme") } )),
    NodeType::normal, 350, 275)), vd_2, vd_3, g);

  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetThreeNodeTwoEdgeNoCenter() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(
    Node(Concept("one", Examples( { Example("unos"), Example("een") } )),
    NodeType::normal, 100, 200), g);
  const auto vd_2 = AddVertex(
    Node(Concept("two", Examples( { Example("Zwei"), Example("twee") } )),
    NodeType::normal, 300, 250), g);
  const auto vd_3 = AddVertex(
    Node(Concept("three", Examples( { Example("drie"), Example("Drei") } )),
    NodeType::normal, 500, 350), g);
  AddEdge(Edge(Node(Concept("first",
    Examples( { Example("erster"), Example("premier") } )),
    NodeType::normal, 150, 225)), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("second",
    Examples( { Example("zweite"), Example("deuxieme") } )),
    NodeType::normal, 350, 275)), vd_2, vd_3, g);
  return g;
}


ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetTwoNodeOneEdge() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_1 = AddVertex(
    Node(Concept("center"), NodeType::center, 100, 200), g);
  const auto vd_2 = AddVertex(
    Node(Concept("one", Examples( { Example("unos"), Example("een") } )),
    NodeType::normal, 300, 250), g);
  AddEdge(
    Edge(
      Node(
        Concept( "first"),
        NodeType::normal,
        150, 225
      ),
      true, //Head arrow
      false
    ), vd_1, vd_2, g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetTwoNodeOneEdgeNoCenter() const noexcept
{
  ConceptMap g;
  //These coordinats are tested as such
  const auto vd_2 = AddVertex(
    Node(Concept("one", Examples( { Example("unos"), Example("een") } )),
    NodeType::normal, 300, 250), g);
  const auto vd_3 = AddVertex(
    Node(Concept("two", Examples( { Example("Zwei"), Example("twee") } )),
    NodeType::normal, 500, 350), g);
  AddEdge(Edge(Node(Concept("second",
    Examples( { Example("zweite"), Example("deuxieme") } )),
    NodeType::normal, 350, 275)), vd_2, vd_3, g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetWithExamplesWithCompetencies(
  const std::vector<Competency>& competencies
) const noexcept
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("A center node is ..."), g);
  Node n(Concept("not me"));
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
  SetExamples(n, Examples(examples));

  const auto vd_2 = AddVertex(n, g);

  AddEdge(Edge(), vd_1, vd_2, g);

  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetNasty0() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(CenterNodeFactory().GetNastyTest(0), g);
  const auto vd_2 = AddVertex(NodeFactory().GetNastyTest(0), g);
  const auto vd_3 = AddVertex(NodeFactory().GetNastyTest(1), g);
  const auto vd_4 = AddVertex(NodeFactory().GetNastyTest(2), g);
  AddEdge(EdgeFactory().GetNastyTest(0), vd_1, vd_2,g);
  AddEdge(EdgeFactory().GetNastyTest(1), vd_2, vd_3,g);
  AddEdge(EdgeFactory().GetNastyTest(2), vd_3, vd_1,g);
  AddEdge(EdgeFactory().GetNastyTest(0), vd_4, vd_1,g);
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
  AddEdge(Edge(Node(Concept("prerequisite", examples_1)), true), vd_2, vd_1,g);
  AddEdge(Edge(Node(Concept("strengthen", examples_2)), true, true), vd_2, vd_3,g);
  return Reposition(g);
}

ribi::cmap::ConceptMap
ribi::cmap::ConceptMapFactory::GetRateConceptTallyDialogExample293() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(
    Node(
      Concept(
        "My focal concept",
        Examples( { Example("My focal concept's example") } ),
        true //is complex
      )
    ),
    g
  );
  const auto vd_2 = AddVertex(
    Node(Concept("My first other concept")),
    g
  );
  const auto vd_3 = AddVertex(
    Node(Concept("My second other concept")),
    g
  );
  AddEdge(
    Edge(
      Node(
        Concept(
          "my first relation",
          Examples( { Example("my first relation's example") } )
        )
      )
    ),
    vd_1,
    vd_2,
    g
  );
  AddEdge(
    Edge(
      Node(
        Concept(
          "my second relation",
          Examples( { Example("my second relation's example") } )
        )
      )
    ),
    vd_1,
    vd_3,
    g
  );
  return Reposition(g);
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
  AddEdge(Edge(Node(Concept("prerequisite", examples_edge_1)), true), vd_2, vd_1,g);
  AddEdge(Edge(Node(Concept("strengthen", examples_edge_2)), true, true), vd_2, vd_3,g);
  return Reposition(g);
}

std::vector<ribi::cmap::ConceptMap >
ribi::cmap::ConceptMapFactory::GetAllTests() const noexcept
{
  std::vector<ConceptMap> v{
    ConceptMap(),
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

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetRated() const noexcept //!OCLINT Indeed a long function, as the concept map is complex
{
  ConceptMap g;

  const auto vd_1 = AddVertex(CenterNodeFactory().Create(
    Concept(
      "As a student teacher of English as a foreign language, "
      "I have knowledge of ...")
    ),
    g
  );
  const auto vd_3 = AddVertex(
    Node(
      Concept(
        "My own professional development",
        Examples (
          {
            Example("Keep your portfolio posted up", Competency::ti_knowledge, false, true, true),
            Example("Reflection", Competency::ti_knowledge, true, true, true)
          }
        ),
        true,
        1,
        1,
        1
      )
    ),
    g
  );
  const auto vd_4 = AddVertex(
    Node(
      Concept(
        "Myself as a teacher",
        Examples(),
        false,
        0,
        0,
        0
      )
    ),
    g
  );

  const auto vd_2 = AddVertex(
    Node(
      Concept(
        "Profession in general",
        Examples(),
        false,
        0,
        0,
        0
      )
    ),
    g
  );
  const auto vd_5 = AddVertex(
    Node(
      Concept(
        "Pupils",
        Examples(),
        false,
        0,
        0,
        0
      )
    ),
    g
  );
  const auto vd_6 = AddVertex(
    Node(
      Concept(
        "Myself as a person",
        Examples(),
        false,
        0,
        0,
        0
      )
    ),
    g
  );
  //Connected to center node
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_3, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_5, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_6, g);
  AddEdge(Edge(Node(Concept("affects")), true, false), vd_2, vd_4, g);
  AddEdge(Edge(Node(Concept("is connected with")), true, false), vd_3, vd_4, g);
  AddEdge(Edge(Node(Concept("is connected with")), true, false), vd_3, vd_6, g);
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetUnrated() const noexcept
{
  ConceptMap g;
  const auto vd_1 = AddVertex(
    Node(
      Concept(
      "As a student teacher of English as a foreign language, "
      "I have knowledge of ..."
      ),
      NodeType::center
    ),
    g
  );
  const auto vd_3 = AddVertex(
    Node(
      Concept(
        "My own professional development",
        Examples(
          {
            Example("Keep your portfolio posted up"),
            Example("Reflection")
          }
        )
      )
    ),
    g
  );
  const auto vd_4 = AddVertex(Node(Concept("Myself as a teacher")), g);
  const auto vd_2 = AddVertex(
    NodeFactory().CreateFromStrings(
      "Profession in general",
      { "An example" }
    ), g);
  const auto vd_5 = AddVertex(NodeFactory().CreateFromStrings("Pupils"), g);
  const auto vd_6 = AddVertex(NodeFactory().CreateFromStrings("Myself as a person"), g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_2, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_3, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_5, g);
  AddEdge(Edge(Node(Concept("")), true, false), vd_1, vd_6, g);
  AddEdge(Edge(Node(Concept("affects")), true, false), vd_2, vd_4, g);
  AddEdge(Edge(Node(Concept("is connected with")), true, false), vd_3, vd_4, g);
  AddEdge(Edge(Node(Concept("is connected with")), true, false), vd_3, vd_6, g);
  return Reposition(g);
}

ribi::cmap::ConceptMap ribi::cmap::Reposition(ConceptMap& g)
{
  //Reposition the nodes
  {
    auto nodes = GetNodes(g);
    const int n_center{CountCenterNodes(nodes)};
    const int n_normal{static_cast<int>(nodes.size()) - n_center};
    double delta_angle{
      2.0 * boost::math::constants::pi<double>() / static_cast<double>(n_normal)
    };
    double angle{-delta_angle / 3.0}; //Gives a nice twist to a
    for (Node& node: nodes)
    {
      if (IsCenterNode(node))
      {
        node.SetPos(0.0, 0.0);
      }
      else
      {
        const double r{250.0}; //ray, half of diameter
        const double x{ std::sin(angle) * r};
        const double y{-std::cos(angle) * r};
        node.SetPos(x, y);
        angle += delta_angle;
      }
    }

    ::set_my_bundled_vertexes(g, nodes);
  }
  //Reposition edges
  {
    const auto eip = boost::edges(g);
    const auto ei_end = eip.second;
    for (auto ei = eip.first; ei != ei_end; ++ei)
    {
      const EdgeDescriptor ed = *ei;
      const Node& from = g[boost::source(ed, g)];
      const Node& to = g[boost::target(ed, g)];
      Edge& edge = g[ed];
      SetX(edge, 0.5 * (from.GetX() + to.GetX()));
      SetY(edge, 0.5 * (from.GetY() + to.GetY()));
    }
  }
  return g;
}
