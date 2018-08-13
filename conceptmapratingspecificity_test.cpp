#include "conceptmapratingspecificity.h"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_comparison)
{
  const auto r = CreateDefaultRatingSpecificity();
  const auto s = CreateTestRatingSpecificity();
  BOOST_CHECK_EQUAL(r, r);
  BOOST_CHECK_EQUAL(s, s);
  BOOST_CHECK_NE(r, s);
  BOOST_CHECK_NE(s, r);
}
