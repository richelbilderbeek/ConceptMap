#include "conceptmapnode.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapcenternodefactory.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_default_should_match)
{
  using namespace ribi::cmap;
  const Node a;
  const Node b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_tests_should_mis_and_match)
{
  using namespace ribi::cmap;
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

BOOST_AUTO_TEST_CASE(ribi_cmap_node_set_and_get_x)
{
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;
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
  using namespace ribi::cmap;

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
  using namespace ribi::cmap;

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
  using namespace ribi::cmap;

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
  using namespace ribi::cmap;

  auto node = NodeFactory().GetNasty0();
  const auto concept = ConceptFactory().Get2();
  assert(node.GetConcept() != concept);
  node.SetConcept(concept);

  BOOST_CHECK_EQUAL(node.GetConcept(), concept);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_to_str)
{
  using namespace ribi::cmap;

  BOOST_CHECK(
    !NodeFactory().GetNasty0().ToStr().empty()
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_xml_to_node)
{
  using namespace ribi::cmap;

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
