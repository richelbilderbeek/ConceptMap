#include "conceptmapcompetency.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_concept_map_competency_to_str)
{
  const ribi::cmap::Competency c = ribi::cmap::Competency::misc;
  const std::string s = ribi::cmap::CompetencyToStr(c);
  const ribi::cmap::Competency d = ribi::cmap::StrToCompetency(s);
  std::stringstream t;
  t << d;
  BOOST_CHECK(!t.str().empty());
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_competency_to_str_dutch)
{
  const ribi::cmap::Competency c = ribi::cmap::Competency::misc;
  const std::string s = ribi::cmap::CompetencyToStrDutch(c);
  BOOST_CHECK_EQUAL(s, "Overige");
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_competency_to_str_dutch_short)
{
  const ribi::cmap::Competency c = ribi::cmap::Competency::misc;
  const std::string s = ribi::cmap::CompetencyToStrDutchShort(c);
  BOOST_CHECK_EQUAL(s, "OV");
}

