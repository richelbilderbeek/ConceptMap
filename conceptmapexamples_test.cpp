#include "conceptmapexamples.h"

#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_operator_equals_and_not)
{
  //Test of operator== and operator!=

  const int sz = ExamplesFactory().GetNumberOfTests();
  for (int i=0; i!=sz; ++i)
  {
    const Examples a = ExamplesFactory().GetTest(i);
          Examples b = ExamplesFactory().GetTest(i);
    BOOST_CHECK(a == a);
    BOOST_CHECK(a == b);
    BOOST_CHECK(b == a);
    BOOST_CHECK(b == b);
    for (int j=0; j!=sz; ++j)
    {
      const Examples c = ExamplesFactory().GetTest(j);
            Examples d = ExamplesFactory().GetTest(j);
      BOOST_CHECK(c == c);
      BOOST_CHECK(c == d);
      BOOST_CHECK(d == c);
      BOOST_CHECK(d == d);
      if (i==j)
      {
        BOOST_CHECK(a == c); BOOST_CHECK(a == d);
        BOOST_CHECK(b == c); BOOST_CHECK(b == d);
        BOOST_CHECK(c == a); BOOST_CHECK(c == b);
        BOOST_CHECK(d == a); BOOST_CHECK(d == b);
      }
      else
      {
        BOOST_CHECK(a != c); BOOST_CHECK(a != d);
        BOOST_CHECK(b != c); BOOST_CHECK(b != d);
        BOOST_CHECK(c != a); BOOST_CHECK(c != b);
        BOOST_CHECK(d != a); BOOST_CHECK(d != b);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_const_index_operator)
{

  const Examples examples( { Example("A"), Example("B") } );
  BOOST_CHECK_EQUAL(examples[0], examples[0]);
  BOOST_CHECK_EQUAL(examples[1], examples[1]);
  BOOST_CHECK_NE(examples[0], examples[1]);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_nonconst_index_operator)
{

  const Example a("A");
  Examples examples( { a } );
  assert(examples[0] == a);
  examples[0] = Example("B");
  BOOST_CHECK_NE(examples[0], a);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_empty_examples_to_xml)
{

  const std::string xml = "<examples></examples>";
  const auto examples = XmlToExamples(xml);
  BOOST_CHECK(examples.Get().empty());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_to_xml_and_examples)
{

  const std::vector<Examples> v = ExamplesFactory().GetTests();
  std::for_each(v.begin(),v.end(),
    [](const Examples& e)
    {
      const std::string s { ToXml(e) };
      const Examples f(XmlToExamples(s));
      BOOST_CHECK_EQUAL(e, f);
    }
  );

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_nasty_examples_to_xml_and_examples)
{

  const std::vector<Examples> v = ExamplesFactory().GetNastyTests();
  std::for_each(v.begin(),v.end(),
    [](const Examples& e)
    {
      const std::string s { ToXml(e) };
      const Examples f(XmlToExamples(s));
      BOOST_CHECK_EQUAL(e, f);
    }
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_to_xml_and_examples_thorough)
{

  const std::vector<Examples> v = ExamplesFactory().GetTests();
  const int sz = boost::numeric_cast<int>(v.size());
  for (int i=0; i!=sz; ++i)
  {
    const Examples& e = v[i];
    const std::string s { ToXml(e) };
    for (int j=0; j!=sz; ++j)
    {
      const Examples& f = v[j];
      const std::string t { ToXml(f) };
      if (i == j)
      {
        BOOST_CHECK(e == f);
        BOOST_CHECK(s == t);
      }
      else
      {
        BOOST_CHECK(e != f);
        BOOST_CHECK(s != t);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_unrated_and_rated_are_different)
{

  const Example a("1",Competency::misc);
  const Example b("1",Competency::misc);
  const Example c("1",Competency::uninitialized);
  BOOST_CHECK(a == a); BOOST_CHECK(a == b); BOOST_CHECK(a != c);
  BOOST_CHECK(b == a); BOOST_CHECK(b == b); BOOST_CHECK(b != c);
  BOOST_CHECK(c != a); BOOST_CHECK(c != b); BOOST_CHECK(c == c);
  std::vector<Example> v; v.push_back(a);
  std::vector<Example> w; w.push_back(b);
  std::vector<Example> x; x.push_back(c);
  const Examples d(v);
  const Examples e(w);
  const Examples f(x);
  BOOST_CHECK(d == d); BOOST_CHECK(d == e); BOOST_CHECK(d != f);
  BOOST_CHECK(e == d); BOOST_CHECK(e == e); BOOST_CHECK(e != f);
  BOOST_CHECK(f != d); BOOST_CHECK(f != e); BOOST_CHECK(f == f);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_stream_once)
{

  const Examples e = ExamplesFactory().GetTest(1);
  std::stringstream s;
  s << e;
  Examples f;
  BOOST_CHECK(e != f);
  s >> f;
  BOOST_CHECK_EQUAL(e, f);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_stream_twice)
{

  const Examples e = ExamplesFactory().GetTest(2);
  const Examples f = ExamplesFactory().GetTest(3);
  std::stringstream s;
  s << e << " " << f;
  Examples g;
  Examples h;
  s >> g >> h;
  BOOST_CHECK_EQUAL(e, g);
  BOOST_CHECK_EQUAL(f, h);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_stream_nasty_once)
{

  for (const Examples e: ExamplesFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Examples f;
    BOOST_CHECK_NE(e, f);
    s >> f;
    BOOST_CHECK_EQUAL(e, f);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_stream_nasty_twice)
{

  for (const Examples e: ExamplesFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Examples g;
    Examples h;
    s >> g >> h;
    BOOST_CHECK_EQUAL(e, g);
    BOOST_CHECK_EQUAL(e, h);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_to_examples)
{

  BOOST_CHECK_THROW(
    XmlToExamples("too short"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToExamples("no examples opening tag"),
    std::invalid_argument
  );

  BOOST_CHECK_THROW(
    XmlToExamples("<examples> no examples closing tag"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(XmlToExamples(ToXml(ExamplesFactory().Get0())));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_CountExamplesRated)
{
  const Example rated("rated", Competency::profession);
  const Example unrated("unrated", Competency::uninitialized);
  BOOST_CHECK_EQUAL(
    0, CountExamplesRated(Examples( { unrated, unrated} ))
  );
  BOOST_CHECK_EQUAL(
    1, CountExamplesRated(Examples( { unrated, rated} ))
  );
  BOOST_CHECK_EQUAL(
    1, CountExamplesRated(Examples( { rated, unrated} ))
  );
  BOOST_CHECK_EQUAL(
    2, CountExamplesRated(Examples( { rated, rated} ))
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_to_str)
{
  BOOST_CHECK(
    !Examples( { Example(), Example() } ).ToStr().empty()
  );
}
