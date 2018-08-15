#include "conceptmapnode.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapcenternodefactory.h"

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_default_should_match)
{
  const Node a;
  const Node b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_tests_should_mis_and_match)
{
  const Node a = NodeFactory().GetTest(1);
  const Node b = NodeFactory().GetTest(2);
  BOOST_CHECK(a != b);
  Node c(a);
  BOOST_CHECK(c == a);
  BOOST_CHECK(c != b);
  c = b;
  BOOST_CHECK(c != a);
  BOOST_CHECK(c == b);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_assign_id)
{
  {
    //Do assign an ID if there is none
    const int no_id{-1};
    const Node node(Concept(), NodeType::normal, 0.0, 0.0, no_id);
    BOOST_CHECK(node.GetId() >= 0);
  }
  {
    //Do not assign an ID if there is one
    const int any_id{42};
    const Node node(Concept(), NodeType::normal, 0.0, 0.0, any_id);
    BOOST_CHECK_EQUAL(node.GetId(), any_id);
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_set_and_get_x)
{

  Node a;
  a.SetX(1.2);
  BOOST_CHECK(a.GetX() == 1.2);
  a.SetY(3.4);
  BOOST_CHECK(a.GetX() == 1.2);
  BOOST_CHECK(a.GetY() == 3.4);
  a.SetX(5.6);
  BOOST_CHECK(a.GetX() == 5.6);
  BOOST_CHECK(a.GetY() == 3.4);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_stream_one)
{

  Node a{NodeFactory().GetTest(1)};
  std::stringstream s;
  s << a;
  Node b;
  s >> b;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(a, b, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_to_xml_and_node)
{

  const std::vector<Node> v = Node::GetTests();
  std::for_each(v.begin(),v.end(),
    [](const Node& node)
    {
      //Test copy constructor
      const Node c(node);
      BOOST_CHECK(node == c);
      const std::string s{ToXml(c)};
      const Node d = XmlToNode(s);
      const double tolerance{0.001};
      BOOST_CHECK(HasSimilarData(c, d, tolerance));
    }
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_has_same_content)
{

  {
    {
      const Concept c(ConceptFactory().Create("1"));
      const Concept d(ConceptFactory().Create("1"));
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a, b));
    }
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      const Concept c = ConceptFactory().Create(
        "1", { {"2", Competency::uninitialized} }
      );
      const Concept d = ConceptFactory().Create(
        "1", { {"2", Competency::uninitialized} }
      );
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
    }

    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
      const double tolerance{0.001};
      BOOST_CHECK(HasSimilarData(a, b, tolerance));
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create(
        "1",
        {
          {"3", Competency::uninitialized},{"2", Competency::uninitialized}
        }
      );
      BOOST_CHECK(c != d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(!HasSameContent(a,b) && "Order in examples is important and cannot be shuffled");
      BOOST_CHECK(a != b);
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create("1", { {"2", Competency::uninitialized} } );
      BOOST_CHECK(c != d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(a != b);
      BOOST_CHECK(!HasSameContent(a,b));
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_concept_factory_reproductions)
{

  const int sz = static_cast<int>(ConceptFactory().GetTests().size());
  for (int i=0; i!=sz; ++i)
  {
    BOOST_CHECK(i < static_cast<int>(ConceptFactory().GetTests().size()));
    const Concept c = ConceptFactory().GetTests()[i];
    const Concept d = ConceptFactory().GetTests()[i];
    BOOST_CHECK(c == d);
    const Node a{c};
    const Node b{d};
    const double tolerance{0.001};
    BOOST_CHECK(HasSameContent(a, b));
    BOOST_CHECK(HasSimilarData(a, b, tolerance));
    BOOST_CHECK(a != b); //Different IDs
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_concept_factory_reproductions_too)
{

  const int sz = static_cast<int>(ConceptFactory().GetTests().size());
  for (int i=0; i!=sz; ++i)
  {
    for (int j=0; j!=sz; ++j)
    {
      BOOST_CHECK(i < static_cast<int>(ConceptFactory().GetTests().size()));
      const Concept c = ConceptFactory().GetTests()[i];
      const Concept d = ConceptFactory().GetTests()[j];
      if (i!=j)
      {
        BOOST_CHECK(c != d);
        const Node a{c};
        const Node b{d};
        BOOST_CHECK(!HasSameContent(a,b));
        const double tolerance{0.001};
        BOOST_CHECK(!HasSimilarData(a, b, tolerance));
        BOOST_CHECK(a != b); //Different IDs
      }
      else
      {
        BOOST_CHECK(c == d);
        const Node a{c};
        const Node b{d};
        BOOST_CHECK(HasSameContent(a,b));
        const double tolerance{0.001};
        BOOST_CHECK(HasSimilarData(a, b, tolerance));
        BOOST_CHECK(a != b); //Different IDs
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_stream_twice)
{

  const Node e = NodeFactory().GetTest(1);
  const Node f = NodeFactory().GetTest(2);
  std::stringstream s;
  s << e << " " << f;
  Node g;
  Node h;
  s >> g >> h;
  const double tolerance{0.001};
  BOOST_CHECK(HasSimilarData(e, g, tolerance));
  BOOST_CHECK(HasSimilarData(f, h, tolerance));
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_stream_nasty_once)
{

  for (const Node e: NodeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Node f;
    BOOST_CHECK_NE(e, f);
    s >> f;

    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(e, f, tolerance));
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_stream_nasty_twice)
{

  for (const Node e: NodeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Node g;
    Node h;
    s >> g >> h;

    const double tolerance{0.001};
    BOOST_CHECK(HasSimilarData(e, g, tolerance));
    BOOST_CHECK(HasSimilarData(e, h, tolerance));
  }
}


BOOST_AUTO_TEST_CASE(ribi_cmap_node_extract_is_center_node_from_xml)
{


  BOOST_CHECK_NO_THROW(
    ExtractIsCenterNodeFromXml(ToXml(CenterNodeFactory().GetNasty0()))
  );

  BOOST_CHECK_THROW(
    ExtractIsCenterNodeFromXml("no is_center_node tag"),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_extract_x_from_xml)
{


  BOOST_CHECK_NO_THROW(
    ExtractXfromXml(ToXml(CenterNodeFactory().GetNasty0()))
  );

  BOOST_CHECK_THROW(
    ExtractXfromXml("no x tag"),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_extract_y_from_xml)
{


  BOOST_CHECK_NO_THROW(
    ExtractYfromXml(ToXml(CenterNodeFactory().GetNasty0()))
  );

  BOOST_CHECK_THROW(
    ExtractYfromXml("no y tag"),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_set_concept)
{


  auto node = NodeFactory().GetNasty0();
  const auto concept = ConceptFactory().Get2();
  assert(node.GetConcept() != concept);
  node.SetConcept(concept);

  BOOST_CHECK_EQUAL(node.GetConcept(), concept);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_to_str)
{


  BOOST_CHECK(
    !NodeFactory().GetNasty0().ToStr().empty()
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_xml_to_node)
{


  BOOST_CHECK_THROW(
    XmlToNode("too short"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToNode("no node starting tag"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToNode("<node> no node ending tag"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    XmlToNode(ToXml(NodeFactory().GetNasty2()))
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_get_example_const)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  const Node node(Concept("", Examples( { Example(text_1), Example(text_2) } )));
  BOOST_CHECK_EQUAL(GetExample(node, 0).GetText(), text_1);
  BOOST_CHECK_EQUAL(GetExample(node, 1).GetText(), text_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_get_example_non_const)
{
  const std::string text_1{"A"};
  const std::string text_2{"B"};
  Node node(Concept("", Examples( { Example(text_1), Example(text_2) } )));
  const std::string text_3{"New"};
  GetExample(node, 0) = Example(text_3);
  BOOST_CHECK_EQUAL(GetExample(node, 0).GetText(), text_3);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_node_set_is_complex)
{
  Node node;
  SetIsComplex(node, true);
  BOOST_CHECK(GetIsComplex(node));
  SetIsComplex(node, false);
  BOOST_CHECK(!GetIsComplex(node));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_node_get_rating_complexity)
{
  Node node;
  for(int x = 0; x != 3; ++x)
  {
    SetRatingComplexity(node, x);
    BOOST_CHECK_EQUAL(GetRatingComplexity(node), x);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_node_get_rating_concreteness)
{
  Node node;
  for(int c = 0; c != 3; ++c)
  {
    SetRatingConcreteness(node, c);
    BOOST_CHECK_EQUAL(GetRatingConcreteness(node), c);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_node_get_rating_specificity)
{
  Node node;
  for(int s = 0; s != 3; ++s)
  {
    SetRatingSpecificity(node, s);
    BOOST_CHECK_EQUAL(GetRatingSpecificity(node), s);
  }
}
