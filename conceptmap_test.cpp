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
#include "custom_vertex_invariant.h"
#include "fileio.h"
#include "install_vertex_custom_type.h"
#include "get_my_custom_vertex.h"
#include "set_my_custom_vertex.h"
#include "is_custom_vertices_isomorphic.h"

#include "make_custom_and_selectable_vertices_writer.h"
#include "my_custom_vertex.h"

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
    "<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>"
  };
  std::stringstream t;
  t << s;
  Node n;
  t >> n;
  BOOST_CHECK(n.GetConcept().GetName() == "A");
  BOOST_CHECK(ToXml(n) == s);
  ConceptMap g;
  const auto vd = boost::add_vertex(g);
  set_my_custom_vertex(n, vd, g);
  const std::string d{ToDot(g)};
  const std::string dot{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>\", regular=\"0\"];\n"
    "}"
  };
  if (d != dot)
  {
    std::cerr << dot << '\n';
    std::cerr << d << '\n';
  }
  BOOST_CHECK(d == dot); //
  ConceptMap c{DotToConceptMap(dot)};
  BOOST_CHECK(boost::num_edges(c) == 0);
  BOOST_CHECK(boost::num_vertices(c) == 1);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_dot_conversion)
{
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string dot{ToDot(c)};
    ConceptMap d{DotToConceptMap(dot)};
    BOOST_CHECK(c == d);
    const std::string dot2{ToDot(d)};
    BOOST_CHECK(dot == dot2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_conversion)
{
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string xml{ToXml(c)};
    ConceptMap d{XmlToConceptMap(xml)};
    BOOST_CHECK(c == d);
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
  using namespace ribi::cmap;
  std::string s{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>\", regular=\"0\"];\n"
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
  using namespace ribi::cmap;
  ConceptMap a{ConceptMapFactory().Get1()};
  std::stringstream s;
  s << a;
  ConceptMap b;
  s >> b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_objects)
{
  using namespace ribi::cmap;
  const ConceptMap e = ConceptMapFactory().Get1();
  const ConceptMap f = ConceptMapFactory().Get2();
  std::stringstream s;
  s << e << f;
  ConceptMap g;
  ConceptMap h;
  s >> g >> h;
  BOOST_CHECK(e == g);
  BOOST_CHECK(f == h);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_one_nasty_object)
{
  using namespace ribi::cmap;
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
    BOOST_CHECK(GetSortedEdges(e) == GetSortedEdges(f));
    BOOST_CHECK(e == f);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_nasty_objects)
{
  using namespace ribi::cmap;
  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    ConceptMap g;
    ConceptMap h;
    s >> g >> h;
    BOOST_CHECK(e == g);
    BOOST_CHECK(e == h);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_and_load_nasty_test)
{
  using namespace ribi::cmap;
  for (const ConceptMap c: ConceptMapFactory().GetNastyTests())
  {
    const auto filename = ribi::fileio::FileIo().GetTempFileName();
    SaveToFile(c, filename);
    const ConceptMap d = LoadFromFile(filename);
    ribi::fileio::FileIo().DeleteFile(filename);
    BOOST_CHECK(c == d);
  }
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_count_center_nodes)
{
  using namespace ribi::cmap;
  for (const ConceptMap& map: ConceptMapFactory().GetAllTests())
  {
    BOOST_CHECK(CountCenterNodes(map) == 0 || CountCenterNodes(map) == 1);
  }
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_find_center_node)
{
  using namespace ribi::cmap;

  //Valid concept map
  BOOST_CHECK_NO_THROW(
    FindCenterNode(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    FindCenterNode(ConceptMapFactory().Get0()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_center_node)
{
  using namespace ribi::cmap;

  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetCenterNode(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetCenterNode(ConceptMapFactory().Get0()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_first_edge)
{
  using namespace ribi::cmap;

  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetFirstEdge(ConceptMapFactory().Get3())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetFirstEdge(ConceptMapFactory().Get0()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_focus_name)
{
  using namespace ribi::cmap;

  //Valid concept map
  BOOST_CHECK_NO_THROW(
    GetFocusName(ConceptMapFactory().Get1())
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetFocusName(ConceptMapFactory().Get0()),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_from)
{
  using namespace ribi::cmap;

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
      ConceptMapFactory().Get0()
    ),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_get_to)
{
  using namespace ribi::cmap;

  //Valid
  BOOST_CHECK_NO_THROW(
    GetTo(
      GetFirstEdge(ConceptMapFactory().Get3()),
      ConceptMapFactory().Get3()
    )
  );

  //Empty concept map
  BOOST_CHECK_THROW(
    GetTo(
      GetFirstEdge(ConceptMapFactory().Get3()),
      ConceptMapFactory().Get0()
    ),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_remove_first_node)
{
  using namespace ribi::cmap;
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

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_to_image)
{
  using namespace ribi::cmap;
  const ribi::FileIo f;
  const std::string filename{"ribi_concept_map_save_to_image.png"};

  if (f.IsRegularFile(filename))
  {
    f.DeleteFile(filename);
  }
  assert(!f.IsRegularFile(filename));
  BOOST_TEST_CHECKPOINT("if error 'child has exited' then install GraphViz");
  SaveToImage(ConceptMapFactory().Get3(), filename);
  BOOST_CHECK(f.IsRegularFile(filename));
  f.DeleteFile(filename);
  BOOST_CHECK(!f.IsRegularFile(filename));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_summary_to_image)
{
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
  const ribi::FileIo f;
  const std::string filename{"ribi_concept_map_save_summary_to_image.dot"};

  if (f.IsRegularFile(filename))
  {
    f.DeleteFile(filename);
  }
  assert(!f.IsRegularFile(filename));

  SaveSummaryToFile(ConceptMapFactory().Get3(), filename);
  BOOST_CHECK(f.IsRegularFile(filename));
  f.DeleteFile(filename);
  BOOST_CHECK(!f.IsRegularFile(filename));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_select_random_node)
{
  using namespace ribi::cmap;
  std::mt19937 rng_engine;
  auto good_graph = ConceptMapFactory().Get1();
  auto bad_graph = ConceptMapFactory().Get0();

  BOOST_CHECK_NO_THROW(
    SelectRandomNode(
      good_graph,
      rng_engine
    )
  );

  BOOST_CHECK_THROW(
    SelectRandomNode(
      bad_graph,
      rng_engine
    ),
    std::invalid_argument
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_unselect_edges)
{
  using namespace ribi::cmap;

  BOOST_CHECK_NO_THROW(
    UnselectEdges(ConceptMapFactory().Get3())
  );

  //No edges, sure, no problem
  BOOST_CHECK_NO_THROW(
    UnselectEdges(ConceptMapFactory().Get0())
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_unselect_everything)
{
  using namespace ribi::cmap;

  BOOST_CHECK_NO_THROW(
    UnselectEverything(ConceptMapFactory().Get3())
  );

  //No edges nor node, sure, no problem
  BOOST_CHECK_NO_THROW(
    UnselectEverything(ConceptMapFactory().Get0())
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_to_concept_map)
{
  using namespace ribi::cmap;

  BOOST_CHECK_THROW(
    XmlToConceptMap("too short"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToConceptMap("incorrect starting tag"),
    std::invalid_argument
  );

  const auto g = ConceptMapFactory().Get3();
  const auto xml = ToXml(g);
  const auto h = XmlToConceptMap(xml);
  BOOST_CHECK_EQUAL(g, h);

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_count_selected_edges)
{
  using namespace ribi::cmap;
  BOOST_CHECK_EQUAL(
    CountSelectedEdges(ConceptMapFactory().Get3()),
    0
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_count_selected_nodes)
{
  using namespace ribi::cmap;
  BOOST_CHECK_EQUAL(
    CountSelectedNodes(ConceptMapFactory().Get3()),
    0
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_create_direct_neighbour_concept_maps)
{
  using namespace ribi::cmap;
  BOOST_CHECK_EQUAL(
    CreateDirectNeighbourConceptMaps(ConceptMapFactory().Get3()).size(),
    2
  );
}


BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateRichnessExperimental)
{
  using ribi::cmap::CalculateRichnessExperimental;
  using ribi::cmap::Competency;
  {
    //An empty map has no richness
    const std::map<Competency, int> empty_map;
    BOOST_CHECK_EQUAL(
      CalculateRichnessExperimental(empty_map),
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
      CalculateRichnessExperimental(m), 100
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
      CalculateRichnessExperimental(m), 100
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
      CalculateRichnessExperimental(m), 25
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
      CalculateRichnessExperimental(m), 92
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
      CalculateRichnessExperimental(m), 100
    );
  }

  {
    //uninitialized throws an exception
    const std::map<Competency, int> m =
    {
      { ribi::cmap::Competency::uninitialized, 1 }
    };
    BOOST_CHECK_THROW(
      CalculateRichnessExperimental(m),
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
      CalculateRichnessExperimental(m),
      std::invalid_argument
    );
  }

}




BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateComplexityEstimated_use)
{
  {
    //Concept map with one center node and two normal unconnected nodes
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    assert(boost::num_vertices(g) == 3);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateComplexityEstimated(g), 0);
  }
  {
    //Concept map with one center node and two normal connected nodes
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    const auto vd_from = AddVertex(NodeFactory().GetTest(0),g);
    const auto vd_to = AddVertex(NodeFactory().GetTest(0),g);
    AddEdge(EdgeFactory().GetTest(0), vd_from, vd_to, g);
    assert(boost::num_vertices(g) == 3);
    assert(boost::num_edges(g) == 1);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateComplexityEstimated(g), 100);
  }
  {
    //Concept map with one center node and three normal unconnected nodes
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    assert(boost::num_vertices(g) == 4);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateComplexityEstimated(g), 0);
  }
  {
    //Concept map with one center node and three normal nodes
    //that have one connection
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    const auto vd_from = AddVertex(NodeFactory().GetTest(0),g);
    const auto vd_to = AddVertex(NodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    AddEdge(EdgeFactory().GetTest(0), vd_from, vd_to, g);
    assert(boost::num_vertices(g) == 4);
    assert(boost::num_edges(g) == 1);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateComplexityEstimated(g), 76);
  }
  {
    //Concept map with one center node and three normal nodes
    //that have two connection
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    const auto vd_a = AddVertex(NodeFactory().GetTest(0),g);
    const auto vd_b = AddVertex(NodeFactory().GetTest(0),g);
    const auto vd_c = AddVertex(NodeFactory().GetTest(0),g);
    AddEdge(EdgeFactory().GetTest(0), vd_a, vd_b, g);
    AddEdge(EdgeFactory().GetTest(0), vd_a, vd_c, g);
    assert(boost::num_vertices(g) == 4);
    assert(boost::num_edges(g) == 2);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateComplexityEstimated(g), 90);
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateComplexityEstimated_abuse)
{
  {
    //Cannot calculate the complexity of an empty concept map
    ConceptMap g;
    BOOST_CHECK_THROW(
      CalculateComplexityEstimated(g),
      std::invalid_argument
    );
  }
  {
    //Cannot calculate the complexity of a concept map with one center node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    assert(boost::num_vertices(g) == 1);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_THROW(
      CalculateComplexityEstimated(g),
      std::invalid_argument
    );

  }
  {
    //Cannot calculate the complexity of a concept map with one center node
    //and one normal node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    AddVertex(NodeFactory().GetTest(0),g);
    assert(boost::num_vertices(g) == 2);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_THROW(
      CalculateComplexityEstimated(g),
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

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateConcretenessEstimated_use)
{
  {
    //Concept map with one center node and one normal unconnected node with zero examples
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    Examples examples; //Empty
    node.GetConcept().SetExamples(examples);
    AddVertex(node, g);
    assert(boost::num_vertices(g) == 2);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 0);
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 100 * 0 / (1 + 1 + 0));
  }
  {
    //Concept map with one center node and one normal unconnected node and one example
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    const Examples examples( { Example("Example 1") } );
    node.GetConcept().SetExamples(examples);
    AddVertex(node, g);
    assert(boost::num_vertices(g) == 2);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 50);
  }
  {
    //Concept map with one center node and one normal unconnected node and two examples
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    const Examples examples(
      {
        Example("Example 1"),
        Example("Example 2")
      }
    );
    node.GetConcept().SetExamples(examples);
    AddVertex(node, g);
    assert(boost::num_vertices(g) == 2);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 67);
  }
  {
    //Concept map with one center node and one normal unconnected node and three examples
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node = NodeFactory().GetTest(0);
    const Examples examples(
      {
        Example("Example 1"),
        Example("Example 2"),
        Example("Example 3")
      }
    );
    node.GetConcept().SetExamples(examples);
    AddVertex(node, g);
    assert(boost::num_vertices(g) == 2);
    assert(boost::num_edges(g) == 0);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 75);
  }
  {
    //Concept map with one center node and two normal connected nodes.
    //The edge has zero examples
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node1 = NodeFactory().GetTest(0);
    node1.GetConcept().SetExamples( Examples() );
    Node node2 = NodeFactory().GetTest(0);
    node2.GetConcept().SetExamples( Examples() );
    const auto vd_from = AddVertex(node1, g);
    const auto vd_to = AddVertex(node2, g);
    Edge edge = EdgeFactory().GetTest(0);
    const Examples examples; //Empty
    edge.GetNode().GetConcept().SetExamples(examples);
    AddEdge(edge, vd_from, vd_to, g);
    assert(boost::num_vertices(g) == 3);
    assert(boost::num_edges(g) == 1);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    //c_e =   0 / (  0 +   2 +     1)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 0);
  }
  {
    //Concept map with one center node and two normal connected nodes.
    //The edge has one example
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node1 = NodeFactory().GetTest(0);
    node1.GetConcept().SetExamples( Examples() );
    Node node2 = NodeFactory().GetTest(0);
    node2.GetConcept().SetExamples( Examples() );
    const auto vd_from = AddVertex(node1, g);
    const auto vd_to = AddVertex(node2, g);
    Edge edge = EdgeFactory().GetTest(0);
    const Examples examples(
      {
        Example("Example 1")
      }
    );
    edge.GetNode().GetConcept().SetExamples(examples);
    AddEdge(edge, vd_from, vd_to, g);
    assert(boost::num_vertices(g) == 3);
    assert(boost::num_edges(g) == 1);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    //c_e =   1 / (  1 +   2 +     1)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 25);
  }
  {
    //Concept map with one center node and two normal connected nodes.
    //The edge has two examples
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    Node node1 = NodeFactory().GetTest(0);
    node1.GetConcept().SetExamples( Examples() );
    Node node2 = NodeFactory().GetTest(0);
    node2.GetConcept().SetExamples( Examples() );
    const auto vd_from = AddVertex(node1, g);
    const auto vd_to = AddVertex(node2, g);
    Edge edge = EdgeFactory().GetTest(0);
    const Examples examples(
      {
        Example("Example 1"),
        Example("Example 2")
      }
    );
    edge.GetNode().GetConcept().SetExamples(examples);
    AddEdge(edge, vd_from, vd_to, g);
    assert(boost::num_vertices(g) == 3);
    assert(boost::num_edges(g) == 1);
    assert(CountCenterNodes(g) == 1);
    //c_e = n_e / (n_e + n_n + nrntf)
    //c_e =   2 / (  2 +   2 +     1)
    BOOST_CHECK_EQUAL(CalculateConcretenessEstimated(g), 40);
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_CalculateConcretenessEstimated_abuse)
{
  {
    //Cannot calculate the concreteness of an empty concept map
    ConceptMap g;
    BOOST_CHECK_THROW(
      CalculateConcretenessEstimated(g),
      std::invalid_argument
    );
  }
  {
    //Cannot calculate the concreteness of a concept map with one center node
    ConceptMap g;
    AddVertex(CenterNodeFactory().GetTest(0),g);
    assert(boost::num_vertices(g) == 1);
    assert(CountCenterNodes(g) == 1);
    BOOST_CHECK_THROW(
      CalculateConcretenessEstimated(g),
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
