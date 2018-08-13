#include "conceptmapedge.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapedgefactory.h"

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_copy_constructor)
{
  const auto edge1 = EdgeFactory().GetTest(1);
  const auto edge2(edge1);
  BOOST_CHECK_EQUAL(edge1, edge2);
  BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
  BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_assignment_operator)
{
  const auto edge1 = EdgeFactory().GetTest(0);
  auto edge2 = EdgeFactory().GetTest(1);
  BOOST_CHECK_NE(edge1, edge2);
  edge2 = edge1;
  BOOST_CHECK_EQUAL(edge1, edge2);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_operator_is_equal)
{
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    const auto edge2 = EdgeFactory().GetTest(0);
    BOOST_CHECK_NE(edge1, edge2); //Different IDs
  }
  {
    const auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
    BOOST_CHECK_NE(edge1, edge2); //Different IDs
  }
  //Toggle head arrow
  {
    auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK(HasSameData(edge1, edge2));
    edge1.SetHeadArrow(!edge1.HasHeadArrow());
    BOOST_CHECK(!HasSameData(edge1, edge2));
    edge1.SetHeadArrow(!edge1.HasHeadArrow());
    BOOST_CHECK(HasSameData(edge1, edge2));
  }
  //Toggle tail arrow
  {
    auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK(HasSameData(edge1, edge2));
    edge1.SetTailArrow(!edge1.HasTailArrow());
    BOOST_CHECK(!HasSameData(edge1, edge2));
    edge1.SetTailArrow(!edge1.HasTailArrow());
    BOOST_CHECK(HasSameData(edge1, edge2));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_operator_is_not_equal)
{
  const auto edge1 = EdgeFactory().GetTest(0);
  const auto edge2 = EdgeFactory().GetTest(1);
  BOOST_CHECK_NE(edge1, edge2);
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_to_xml_to_edge_easy)
{
  //Can all edges be converted to XML and back?
  const auto edges = EdgeFactory().GetTests();
  for (const auto edge_before: edges)
  {
    const std::string s{ToXml(edge_before)};
    const Edge edge_after{XmlToEdge(s)};
    BOOST_CHECK_EQUAL(ToXml(edge_before), ToXml(edge_after));
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(edge_before, edge_after, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_to_xml_to_edge_nasty)
{
  //Can all edges be converted to XML and back?
  const auto edges = EdgeFactory().GetNastyTests();
  for (const auto edge_before: edges)
  {
    const std::string s{ToXml(edge_before)};
    const Edge edge_after{XmlToEdge(s)};
    BOOST_CHECK_EQUAL(ToXml(edge_before), ToXml(edge_after));
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(edge_before, edge_after, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operator)
{
  const auto edge1 = EdgeFactory().GetTest(0);
  std::stringstream s;
  s << edge1;
  Edge edge2;
  s >> edge2;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(edge1, edge2, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operators)
{
  const Edge e = EdgeFactory().GetTest(1);
  const Edge f = EdgeFactory().GetTest(2);
  std::stringstream s;
  s << e << f;
  Edge g;
  Edge h;
  s >> g >> h;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(e, g, tolerance));
  BOOST_CHECK(HasSimilarData(f, h, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operator_nasty)
{
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Edge f;
    BOOST_CHECK_NE(e, f);
    s >> f;
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(e, f, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operators_nasty)
{
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    Edge g;
    Edge h;
    s >> g >> h;
    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(e, g, tolerance));
    BOOST_CHECK(HasSimilarData(e, h, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_assign_id)
{
  {
    //Do assign an ID if there is none
    const int no_id{-1};
    const Edge edge(Node(), false, false, no_id);
    BOOST_CHECK(edge.GetId() >= 0);
  }
  {
    //Do not assign an ID if there is one
    const int any_id{42};
    const Edge edge(Node(), false, false, any_id);
    BOOST_CHECK_EQUAL(edge.GetId(), any_id);
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_collect_examples_texts_no_example)
{
  const Edge edge;
  const std::vector<std::string> created = CollectExamplesTexts(edge);
  const std::vector<std::string> expected = {};
  BOOST_CHECK_EQUAL_COLLECTIONS(
    std::begin(created),
    std::end(created),
    std::begin(expected),
    std::end(expected)
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_collect_examples_texts_1_example)
{
  const std::string text{"ABC"};
  const Edge edge(Node(Concept("", Examples( { Example(text) } ))));
  const std::vector<std::string> created = CollectExamplesTexts(edge);
  const std::vector<std::string> expected = { text };
  BOOST_CHECK_EQUAL_COLLECTIONS(
    std::begin(created),
    std::end(created),
    std::begin(expected),
    std::end(expected)
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_collect_examples_texts_2_examples)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  const Edge edge(Node(Concept("", Examples( { Example(text_1), Example(text_2) } ))));
  const std::vector<std::string> created = CollectExamplesTexts(edge);
  const std::vector<std::string> expected = { text_1, text_2};
  BOOST_CHECK_EQUAL_COLLECTIONS(
    std::begin(created),
    std::end(created),
    std::begin(expected),
    std::end(expected)
  );
}








BOOST_AUTO_TEST_CASE(ribi_cmap_edge_count_examples_no_example)
{
  const Edge edge;
  BOOST_CHECK_EQUAL(0, CountExamples(edge));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_count_examples_1_example)
{
  const std::string text{"ABC"};
  const Edge edge(Node(Concept("", Examples( { Example(text) } ))));
  BOOST_CHECK_EQUAL(1, CountExamples(edge));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_count_examples_2_examples)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  const Edge edge(Node(Concept("", Examples( { Example(text_1), Example(text_2) } ))));
  BOOST_CHECK_EQUAL(2, CountExamples(edge));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_get_example_const)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  const Edge edge(Node(Concept("", Examples( { Example(text_1), Example(text_2) } ))));
  BOOST_CHECK_EQUAL(GetExample(edge, 0).GetText(), text_1);
  BOOST_CHECK_EQUAL(GetExample(edge, 1).GetText(), text_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_get_example_non_const)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  Edge edge(Node(Concept("", Examples( { Example(text_1), Example(text_2) } ))));
  const std::string text_3{"New"};
  GetExample(edge, 0) = Example(text_3);
  BOOST_CHECK_EQUAL(GetExample(edge, 0).GetText(), text_3);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_edge_get_concept_const)
{
  const Concept c("A");
  const Edge edge(Node(c, NodeType::normal));
  BOOST_CHECK_EQUAL(c, ::ribi::cmap::GetConcept(edge));
}
