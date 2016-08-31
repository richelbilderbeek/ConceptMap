//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
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

BOOST_AUTO_TEST_CASE(ribi_concept_map_is_copy_constructable)
{
  using namespace ribi::cmap;
  const ConceptMap a;
  const ConceptMap b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_operator_is_equal)
{
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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













