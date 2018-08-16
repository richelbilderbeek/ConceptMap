#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/lexical_cast.hpp>

#include "conceptmapcenternodefactory.h"
#include "conceptmapcompetency.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamplevertexinvariant.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmaphelper.h"
#include "conceptmapisexampleverticesisomorphic.h"
#include "conceptmapisexamplesverticesisomorphic.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "fileio.h"
#include "get_my_bundled_edge.h"
#include "get_my_bundled_vertex.h"
#include "set_my_bundled_vertex.h"

//#include "make_bundled_vertices_writer.h"
#include "my_bundled_vertex.h"

#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_concept_map_is_copy_constructable)
{
  const ConceptMap a;
  const ConceptMap b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_operator_is_equal)
{
  const ConceptMap a = ConceptMapFactory().Get1();
  const ConceptMap b = ConceptMapFactory().Get2();
  BOOST_CHECK(a != b);
  ConceptMap c(a);
  BOOST_CHECK(c == a);
  BOOST_CHECK(c != b);
  c = b;
  BOOST_CHECK(c != a);
  BOOST_CHECK(c == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_load_node)
{
  const std::string s{
    "<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><type>normal</type></node>"
  };
  std::stringstream t;
  t << s;
  Node n;
  t >> n;
  BOOST_CHECK(n.GetConcept().GetName() == "A");
  BOOST_CHECK(ToXml(n) == s);
  ConceptMap g;
  const auto vd = boost::add_vertex(g);
  set_my_bundled_vertex(n, vd, g);
  const std::string d{ToDot(g)};
  const std::string dot{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><type>normal</type></node>\"];\n"
    "}"
  };
  BOOST_CHECK(d == dot);
  ConceptMap c{DotToConceptMap(dot)};
  BOOST_CHECK(boost::num_edges(c) == 0);
  BOOST_CHECK(boost::num_vertices(c) == 1);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_dot_conversion_1)
{
  const ConceptMap c{ConceptMapFactory().Get1()};
  const std::string dot{ToDot(c)};
  ConceptMap d{DotToConceptMap(dot)};
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(c, d, tolerance));
  const std::string dot2{ToDot(d)};
  BOOST_CHECK_EQUAL(dot, dot2);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_dot_conversion)
{
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string dot{ToDot(c)};
    ConceptMap d{DotToConceptMap(dot)};
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(c, d, tolerance));
    const std::string dot2{ToDot(d)};
    BOOST_CHECK_EQUAL(dot, dot2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_conversion)
{
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string xml{ToXml(c)};
    ConceptMap d{XmlToConceptMap(xml)};
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(c, d, tolerance));
    const std::string xml2{ToXml(d)};
    BOOST_CHECK(xml == xml2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_streaming_empty_graph)
{
  ConceptMap a;
  std::stringstream s;
  s << a;
  ConceptMap b;
  s >> b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_simple_dot_to_concept_map)
{

  std::string s{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><type>normal</type></node>\"];\n"
    "}"
  };
  {
    std::ofstream f("tmp.dot");
    f << s;
  }
  const ConceptMap c = DotToConceptMap(s);
  BOOST_CHECK(s == ToDot(c));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_single_object)
{

  ConceptMap a{ConceptMapFactory().Get1()};
  std::stringstream s;
  s << a;
  ConceptMap b;
  s >> b;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(a, b, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_objects)
{

  const ConceptMap e = ConceptMapFactory().Get1();
  const ConceptMap f = ConceptMapFactory().Get2();
  std::stringstream s;
  s << e << f;
  ConceptMap g;
  ConceptMap h;
  s >> g >> h;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(e, g, tolerance));
  BOOST_CHECK(HasSimilarData(f, h, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_one_nasty_object)
{

  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    {
      std::ofstream f("tmp.dot");
      f << e;
    }
    std::stringstream s;
    s << e;
    ConceptMap f;
    BOOST_CHECK(e != f);
    s >> f;
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(GetSortedEdges(e), GetSortedEdges(f), tolerance));
    BOOST_CHECK(HasSimilarData(e, f, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_nasty_objects)
{

  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    ConceptMap g;
    ConceptMap h;
    s >> g >> h;

    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(e, g, tolerance));
    BOOST_CHECK(HasSimilarData(e, h, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_and_load_detailed)
{
  const ConceptMap c{ConceptMapFactory().GetThreeNodeTwoEdge()};
  const auto filename = ribi::fileio::FileIo().GetTempFileName();
  SaveToFile(c, filename);
  const ConceptMap d = LoadFromFile(filename);
  ribi::fileio::FileIo().DeleteFile(filename);

  const auto vd1 = boost::vertex(0, d);
  const auto vd2 = boost::vertex(1, d);
  const auto vd3 = boost::vertex(2, d);
  const Node node1 = get_my_bundled_vertex(vd1, d);
  const Node node2 = get_my_bundled_vertex(vd2, d);
  const Node node3 = get_my_bundled_vertex(vd3, d);
  BOOST_CHECK_EQUAL(node1.GetName(), "center");
  BOOST_CHECK_EQUAL(GetX(node1), 100);
  BOOST_CHECK_EQUAL(GetY(node1), 200);
  BOOST_CHECK_EQUAL(node2.GetName(), "one");
  BOOST_CHECK_EQUAL(node2.GetX(), 300);
  BOOST_CHECK_EQUAL(node2.GetY(), 250);
  BOOST_CHECK_EQUAL(node3.GetName(), "two");
  BOOST_CHECK_EQUAL(node3.GetX(), 500);
  BOOST_CHECK_EQUAL(node3.GetY(), 350);
  const Edge edge1 = get_my_bundled_edge(boost::edge(vd1, vd2, d).first, d);
  const Edge edge2 = get_my_bundled_edge(boost::edge(vd2, vd3, d).first, d);
  BOOST_CHECK_EQUAL(GetText(edge1), "first");
  BOOST_CHECK_EQUAL(GetText(edge2), "second");
  BOOST_CHECK_EQUAL(GetX(edge1), 150);
  BOOST_CHECK_EQUAL(GetY(edge1), 225);
  BOOST_CHECK_EQUAL(GetX(edge2), 350);
  BOOST_CHECK_EQUAL(GetY(edge2), 275);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_and_load_nasty_test)
{

  for (const ConceptMap c: ConceptMapFactory().GetNastyTests())
  {
    const auto filename = ribi::fileio::FileIo().GetTempFileName();
    SaveToFile(c, filename);
    const ConceptMap d = LoadFromFile(filename);
    ribi::fileio::FileIo().DeleteFile(filename);

    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(c, d, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_LoadFromFile_on_absent_file)
{
  BOOST_CHECK_THROW(
    LoadFromFile("abs.ent"),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_has_center_node)
{
  BOOST_CHECK_EQUAL(
    false,
    HasCenterNode(
      ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter()
    )
  );

  BOOST_CHECK_EQUAL(
    true,
    HasCenterNode(
      ConceptMapFactory().GetThreeNodeTwoEdge()
    )
  );
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_count_center_nodes)
{
  BOOST_CHECK_EQUAL(
    0,
    CountCenterNodes(
      ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter()
    )
  );

  BOOST_CHECK_EQUAL(
    1,
    CountCenterNodes(
      ConceptMapFactory().GetThreeNodeTwoEdge()
    )
  );
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_find_center_node)
{


  //Valid concept map
  BOOST_CHECK_NO_THROW(
    FindCenterNode(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    FindCenterNode(ConceptMap()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_center_node)
{


  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetCenterNode(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetCenterNode(ConceptMap()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_first_edge)
{


  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetFirstEdge(ConceptMapFactory().Get3())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetFirstEdge(ConceptMap()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_focus_name)
{


  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetFocusName(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetFocusName(ConceptMap()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_from)
{


  const auto g = ConceptMapFactory().Get3();
  const auto edge = GetFirstEdge(g);

  //Valid
  BOOST_CHECK_NO_THROW(
    GetFrom(edge, g)
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetFrom(
      edge,
      ConceptMap()
    ),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_to)
{


  //Valid
  const ConceptMap m{ConceptMapFactory().Get3()};
  BOOST_CHECK_NO_THROW(
    GetTo(
      GetFirstEdge(m),
      m
    )
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetTo(
      GetFirstEdge(m),
      ConceptMap()
    ),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_remove_first_node)
{

  auto g = ConceptMapFactory().Get3();
  assert(boost::num_vertices(g) == 2);

  g = RemoveFirstNode(g);
  g = RemoveFirstNode(g);

  assert(boost::num_vertices(g) == 0);

  //Empty concept map
  BOOST_CHECK_THROW(
    RemoveFirstNode(g),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_summary_to_image)
{
  const ribi::FileIo f;
  const std::string filename{"ribi_concept_map_save_summary_to_image.png"};

  if (f.IsRegularFile(filename))
  {
    f.DeleteFile(filename);
  }
  assert(!f.IsRegularFile(filename));

  SaveSummaryToImage(ConceptMapFactory().Get3(), filename);
  BOOST_CHECK(f.IsRegularFile(filename));
  f.DeleteFile(filename);
  BOOST_CHECK(!f.IsRegularFile(filename));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_summary_to_file)
{

  const ribi::FileIo f;
  const std::string filename{"ribi_concept_map_save_summary_to_image.dot"};

  if (f.IsRegularFile(filename))
  {
    f.DeleteFile(filename);
  }
  assert(!f.IsRegularFile(filename));

  SaveSummaryToFile(ConceptMapFactory().GetAllArrowTypes(), filename);
  BOOST_CHECK(f.IsRegularFile(filename));
  f.DeleteFile(filename);
  BOOST_CHECK(!f.IsRegularFile(filename));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_to_concept_map)
{
  BOOST_CHECK_THROW(
    XmlToConceptMap("too short"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToConceptMap("incorrect starting tag"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToConceptMap("<concept_map>no closing tag"),
    std::invalid_argument
  );

  const auto g = ConceptMapFactory().Get3();
  const auto xml = ToXml(g);
  const auto h = XmlToConceptMap(xml);
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(g, h, tolerance));

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_create_direct_neighbour_concept_maps)
{

  BOOST_CHECK_EQUAL(
    CreateDirectNeighbourConceptMaps(ConceptMapFactory().Get3()).size(),
    2
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateRichness_on_concept_map)
{
  //An empty map has no richness
  BOOST_CHECK_EQUAL(
    CalculateRichness(ConceptMap()),
    0
  );

  //An empty map has no richness
  BOOST_CHECK_NO_THROW(
    CalculateRichness(ConceptMapFactory().GetRated())
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateRichness_on_tallies)
{
  using ribi::cmap::CalculateRichness;
  using ribi::cmap::Competency;
  {
    //An empty map has no richness
    const std::map<Competency, int> empty_map;
    BOOST_CHECK_EQUAL(
      CalculateRichness(empty_map),
      0
    );
  }
  {
    //An even distribution over the domains, 1 scores per competency
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::organisations, 1 },
      { ribi::cmap::Competency::profession, 1 },
      { ribi::cmap::Competency::prof_growth, 1 },
      { ribi::cmap::Competency::social_surroundings, 1 },
      { ribi::cmap::Competency::target_audience, 1 },
      { ribi::cmap::Competency::ti_knowledge, 1 }
    };
    BOOST_CHECK_EQUAL(
      CalculateRichness(m), 100
    );
  }
  {
    //An even distribution over the domains, 10 scores per competency
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::organisations, 10 },
      { ribi::cmap::Competency::profession, 10 },
      { ribi::cmap::Competency::prof_growth, 10 },
      { ribi::cmap::Competency::social_surroundings, 10 },
      { ribi::cmap::Competency::target_audience, 10 },
      { ribi::cmap::Competency::ti_knowledge, 10 }
    };
    BOOST_CHECK_EQUAL(
      CalculateRichness(m), 100
    );
  }
  {
    //Below 12%, a competency does not contribute to b, use 10% here
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::organisations, 9 },
      { ribi::cmap::Competency::profession, 1 },
    };
    // a: 2
    // b: 1
    // r: 100 * (a + b) / 12
    BOOST_CHECK_EQUAL(
      CalculateRichness(m), 25
    );
  }
  {
    //Above 25%, a competency does not contribute to b, use 33% here
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::organisations, 33 },
      { ribi::cmap::Competency::profession, 13 },
      { ribi::cmap::Competency::prof_growth, 14 },
      { ribi::cmap::Competency::social_surroundings, 13 },
      { ribi::cmap::Competency::target_audience, 13 },
      { ribi::cmap::Competency::ti_knowledge, 14 }
    };
    // a: 6 (all are present
    // b: 5 (all but one are between 12%-25%)
    // r: 100 * (a + b) / 12 = 92
    BOOST_CHECK_EQUAL(
      CalculateRichness(m), 92
    );
  }
  {
    //misc is ignored without changing the calculation
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::organisations, 10 },
      { ribi::cmap::Competency::profession, 10 },
      { ribi::cmap::Competency::prof_growth, 10 },
      { ribi::cmap::Competency::social_surroundings, 10 },
      { ribi::cmap::Competency::target_audience, 10 },
      { ribi::cmap::Competency::ti_knowledge, 10 },
      { ribi::cmap::Competency::misc, 100 }
    };
    BOOST_CHECK_EQUAL(
      CalculateRichness(m), 100
    );
  }

  {
    //uninitialized throws an exception
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::uninitialized, 1 }
    };
    BOOST_CHECK_THROW(
      CalculateRichness(m),
      std::invalid_argument
    );
  }
  {
    //n_competencies throws an exception
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::n_competencies, 1 }
    };
    BOOST_CHECK_THROW(
      CalculateRichness(m),
      std::invalid_argument
    );
  }

}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateComplexityExperimental_use)
{
  {
    //Concept map with a center node and one normal node of complexity zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingComplexity(0);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and one normal node of complexity one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingComplexity(1);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and one normal node of complexity two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingComplexity(2);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      100
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingComplexity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingComplexity(0);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingComplexity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingComplexity(1);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      25
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingComplexity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingComplexity(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and two normal node of complexities one and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingComplexity(1);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingComplexity(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateComplexityExperimental(g),
      75
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateComplexityExperimental_abuse)
{
  {
    //Cannot calculate complexity of an empty concept map
    ConceptMap g;
    BOOST_CHECK_THROW(
      CalculateComplexityExperimental(g),
      std::invalid_argument
    );
  }
  {
    //Cannot calculate complexity of a concept map with only a center node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    BOOST_CHECK_THROW(
      CalculateComplexityExperimental(g),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateConcretenessExperimental_use)
{
  {
    //Concept map with a center node and one normal node of concreteness zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingConcreteness(0);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and one normal node of concreteness one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingConcreteness(1);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and one normal node of concreteness two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingConcreteness(2);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      100
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingConcreteness(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingConcreteness(0);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingConcreteness(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingConcreteness(1);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      25
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingConcreteness(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingConcreteness(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and two normal node of complexities one and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingConcreteness(1);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingConcreteness(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateConcretenessExperimental(g),
      75
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateConcretenessExperimental_abuse)
{
  {
    //Cannot calculate concreteness of an empty concept map
    ConceptMap g;
    BOOST_CHECK_THROW(
      CalculateConcretenessExperimental(g),
      std::invalid_argument
    );
  }
  {
    //Cannot calculate concreteness of a concept map with only a center node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    BOOST_CHECK_THROW(
      CalculateConcretenessExperimental(g),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateSpecificityExperimental_use)
{
  {
    //Concept map with a center node and one normal node of specificity zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingSpecificity(0);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and one normal node of specificity one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingSpecificity(1);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and one normal node of specificity two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    node.GetConcept().SetRatingSpecificity(2);
    AddVertex(node,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      100
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and zero
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingSpecificity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingSpecificity(0);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      0
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and one
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingSpecificity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingSpecificity(1);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      25
    );
  }
  {
    //Concept map with a center node and two normal node of complexities zero and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingSpecificity(0);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingSpecificity(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      50
    );
  }
  {
    //Concept map with a center node and two normal node of complexities one and two
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node_1 = NodeFactory().GetTest(0);
    node_1.GetConcept().SetRatingSpecificity(1);
    AddVertex(node_1,g);
    Node node_2 = NodeFactory().GetTest(0);
    node_2.GetConcept().SetRatingSpecificity(2);
    AddVertex(node_2,g);
    BOOST_CHECK_EQUAL(
      CalculateSpecificityExperimental(g),
      75
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateSpecificityExperimental_abuse)
{
  {
    //Cannot calculate specificity of an empty concept map
    ConceptMap g;
    BOOST_CHECK_THROW(
      CalculateSpecificityExperimental(g),
      std::invalid_argument
    );
  }
  {
    //Cannot calculate specificity of a concept map with only a center node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    BOOST_CHECK_THROW(
      CalculateSpecificityExperimental(g),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CollectCompetencies)
{
  BOOST_CHECK(!CollectCompetenies(ConceptMapFactory().GetRated()).empty());
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CountExamples)
{
  BOOST_CHECK(CountExamples(ConceptMapFactory().GetRated()) > 0);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_GetFirstNode_on_empty_concept_map)
{
  BOOST_CHECK_THROW(
    GetFirstNode(ConceptMap()),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_GetFromTo)
{
  const auto conceptmap = ConceptMapFactory().GetTwoNodeOneEdgeNoCenter();
  const auto nodes = GetFromTo(*boost::edges(conceptmap).first, conceptmap);
  BOOST_CHECK_NE(nodes.first, nodes.second);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_GetNodesSortedByLevel)
{
  //      2 <- 1 <- 0 -> 1 -> 2
  //                |    |
  //                v    v
  //                1    2
  const auto conceptmap = ConceptMapFactory().GetLevel();
  const auto nodes = GetNodesSortedByLevel(conceptmap);
  BOOST_CHECK_EQUAL(nodes[0].GetName(), std::to_string(0));
  BOOST_CHECK_EQUAL(nodes[1].GetName(), std::to_string(1));
  BOOST_CHECK_EQUAL(nodes[2].GetName(), std::to_string(1));
  BOOST_CHECK_EQUAL(nodes[3].GetName(), std::to_string(1));
  BOOST_CHECK_EQUAL(nodes[4].GetName(), std::to_string(2));
  BOOST_CHECK_EQUAL(nodes[5].GetName(), std::to_string(2));
  BOOST_CHECK_EQUAL(nodes[6].GetName(), std::to_string(2));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_IsConnectedTo)
{
  const auto conceptmap = ConceptMapFactory().GetThreeNodeTwoEdge();
  auto vi = boost::vertices(conceptmap).first;
  const auto& first_node = conceptmap[*vi];
  ++vi;
  const auto& second_node = conceptmap[*vi];
  ++vi;
  const auto& third_node = conceptmap[*vi];
  auto ei = boost::edges(conceptmap).first;
  const auto& first_edge = conceptmap[*ei];
  ++ei;
  const auto& second_edge = conceptmap[*ei];
  BOOST_CHECK( IsConnectedTo(first_edge, first_node, conceptmap));
  BOOST_CHECK( IsConnectedTo(first_edge, second_node, conceptmap));
  BOOST_CHECK(!IsConnectedTo(first_edge, third_node, conceptmap));
  BOOST_CHECK(!IsConnectedTo(second_edge, first_node, conceptmap));
  BOOST_CHECK( IsConnectedTo(second_edge, second_node, conceptmap));
  BOOST_CHECK( IsConnectedTo(second_edge, third_node, conceptmap));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_IsCenterNode)
{
  const auto conceptmap = ConceptMapFactory().GetThreeNodeTwoEdge();
  auto vi = boost::vertices(conceptmap).first;
  const auto& first_node = conceptmap[*vi];
  ++vi;
  const auto& second_node = conceptmap[*vi];
  ++vi;
  const auto& third_node = conceptmap[*vi];
  BOOST_CHECK( IsCenterNode(first_node));
  BOOST_CHECK(!IsCenterNode(second_node));
  BOOST_CHECK(!IsCenterNode(third_node));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_IsPrimaryConcept)
{
  const auto conceptmap = ConceptMapFactory().GetThreeNodeTwoEdge();
  auto vi = boost::vertices(conceptmap).first;
  BOOST_CHECK(!IsPrimaryConcept(*vi, conceptmap));
  ++vi;
  BOOST_CHECK(IsPrimaryConcept(*vi, conceptmap));
  ++vi;
  BOOST_CHECK(!IsPrimaryConcept(*vi, conceptmap));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_IsSecondaryConcept)
{
  const auto conceptmap = ConceptMapFactory().GetThreeNodeTwoEdge();
  auto vi = boost::vertices(conceptmap).first;
  BOOST_CHECK(!IsSecondaryConcept(*vi, conceptmap));
  ++vi;
  BOOST_CHECK(!IsSecondaryConcept(*vi, conceptmap));
  ++vi;
  BOOST_CHECK(IsSecondaryConcept(*vi, conceptmap));
}
