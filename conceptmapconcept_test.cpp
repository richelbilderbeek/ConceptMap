#include "conceptmapconcept.h"

#include <boost/test/unit_test.hpp>

#include "conceptmaphelper.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamplesfactory.h"

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_concept_map_copy_constructor)
{
  const Concept c;
  Concept d(c);
  BOOST_CHECK(d == c);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_assignment_operator)
{
  const Concept c("A");
  const Concept d("B");
  BOOST_CHECK(c != d);
  Concept e(c);
  BOOST_CHECK(e == c);
  BOOST_CHECK(e != d);
  e = d;
  BOOST_CHECK(e != c);
  BOOST_CHECK(e == d);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_operator_equals_and_not)
{
  // Test operator== and operator!=
  const int sz = static_cast<int>(ConceptFactory().GetTests().size());
  BOOST_CHECK(sz > 0);
  for (int i=0; i!=sz; ++i)
  {
    const auto tmp_a = ConceptFactory().GetTests();
    const auto tmp_b = ConceptFactory().GetTests();
    const Concept a = tmp_a.at(i);
          Concept b = tmp_b.at(i);
    BOOST_CHECK(a == a);
    BOOST_CHECK(b == a);
    BOOST_CHECK(a == b);
    BOOST_CHECK(b == b);
    for (int j=0; j!=sz; ++j)
    {
      BOOST_CHECK(j < static_cast<int>(ConceptFactory().GetTests().size()));
      const Concept c = ConceptFactory().GetTests().at(j);
            Concept d = ConceptFactory().GetTests().at(j);
      BOOST_CHECK(c == c);
      BOOST_CHECK(d == c);
      BOOST_CHECK(c == d);
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

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_operator_less)
{
  //Test operator<

  {
    // operator< must order by name
    {
      const Concept a = ConceptFactory().Create("1");
            Concept b = ConceptFactory().Create("1");
      const Concept c = ConceptFactory().Create("2");
            Concept d = ConceptFactory().Create("2");
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    //if (verbose) { TRACE("operator< must order by examples' size, sizes 0 versus 1"); }
    {
      const Concept a = ConceptFactory().Create("1");
            Concept b = ConceptFactory().Create("1");
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc} } );
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc} } );
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    //if (verbose) { TRACE("operator< must order by examples' size, sizes 1 versus 2"); }
    {
      const Concept a = ConceptFactory().Create("1", { {"2",Competency::misc} } );
            Concept b = ConceptFactory().Create("1", { {"2",Competency::misc} } );
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    //if (verbose) { TRACE("Check correct ordering for equal examples' size, lexicographically in the 2nd text"); }
    {
      const Concept a = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
            Concept b = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc},{"4",Competency::misc} } ); //!OCLINT
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc},{"4",Competency::misc} } ); //!OCLINT
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_xml_to_concept)
{

  const std::string xml = "<concept><name>TEST</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept>"; //!OCLINT
  const auto concept = XmlToConcept(xml);
  BOOST_CHECK(concept.GetName() == "TEST");
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_stream_once)
{

  const Concept e = ConceptFactory().GetTest(1);
  const Concept f = ConceptFactory().GetTest(2);
  std::stringstream s;
  s << e << " " << f;
  Concept g;
  Concept h;
  s >> g >> h;
  BOOST_CHECK_EQUAL(e, g);
  BOOST_CHECK_EQUAL(f, h);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_nasty_stream_once)
{

  for (const Concept e: ConceptFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Concept f;
    BOOST_CHECK(e != f);
    s >> f;
    BOOST_CHECK_EQUAL(e, f);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_examples_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractExamplesFromXml("No examples here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractExamplesFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_is_complex_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractIsComplexFromXml("No is_complex here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractIsComplexFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_name_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractNameFromXml("No name here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractNameFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_rating_complexity_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractRatingComplexityFromXml("No rating_complexity here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractRatingComplexityFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_rating_concreteness_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractRatingConcretenessFromXml("No rating_concreteness here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractRatingConcretenessFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_extract_rating_specificity_from_xml)
{

  BOOST_CHECK_THROW(
    ExtractRatingSpecificityFromXml("No rating_specificity here"),
    std::invalid_argument
  );

  BOOST_CHECK_NO_THROW(
    ExtractRatingSpecificityFromXml(
      ToXml(ConceptFactory().GetNasty2())
    )
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_examples)
{

  const auto examples = ExamplesFactory().Get0();
  Concept concept = ConceptFactory().GetNasty2();
  assert(concept.GetExamples() != examples);
  concept.SetExamples(examples);
  BOOST_CHECK_EQUAL(examples, concept.GetExamples());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_is_complex_member_functions)
{
  Concept concept;
  concept.SetIsComplex(true);
  BOOST_CHECK(concept.GetIsComplex());
  concept.SetIsComplex(false);
  BOOST_CHECK(!concept.GetIsComplex());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_is_complex_free_functions)
{
  Concept concept;
  SetIsComplex(concept, true);
  BOOST_CHECK(GetIsComplex(concept));
  SetIsComplex(concept, false);
  BOOST_CHECK(!GetIsComplex(concept));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_name)
{

  Concept concept = ConceptFactory().GetNasty2();
  const std::string name{"ribi_concept_map_concept_set_name"};
  concept.SetName(name);
  BOOST_CHECK_EQUAL(concept.GetName(), name);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_rating_complexity)
{

  Concept concept = ConceptFactory().GetNasty2();

  //Use
  const int rating{2};
  concept.SetRatingComplexity(rating);
  BOOST_CHECK_EQUAL(concept.GetRatingComplexity(), rating);

  //Abuse
  BOOST_CHECK_THROW(
    concept.SetRatingComplexity(-4),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_rating_concreteness)
{

  Concept concept = ConceptFactory().GetNasty2();

  //Use
  const int rating{2};
  concept.SetRatingConcreteness(rating);
  BOOST_CHECK_EQUAL(concept.GetRatingConcreteness(), rating);

  //Abuse
  BOOST_CHECK_THROW(
    concept.SetRatingConcreteness(-4),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_set_rating_specificity)
{

  Concept concept = ConceptFactory().GetNasty2();

  //Use
  const int rating{2};
  concept.SetRatingSpecificity(rating);
  BOOST_CHECK_EQUAL(concept.GetRatingSpecificity(), rating);

  //Abuse
  BOOST_CHECK_THROW(
    concept.SetRatingSpecificity(-4),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_to_str)
{

  BOOST_CHECK(
    !ConceptFactory().GetNasty2().ToStr().empty()
  );
}
