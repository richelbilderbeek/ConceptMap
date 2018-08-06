#include "conceptmaprating.h"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_test_default_rating)
{
  const Rating r = CreateDefaultRating();
  for (int n_examples = 0; n_examples != 10; ++n_examples)
  {
    for (int n_edges = 0; n_edges != 10; ++n_edges)
    {
      BOOST_CHECK_EQUAL(
        r.SuggestComplexity(n_edges, n_examples),
        RatingComplexity::SuggestComplexityDefault(n_edges, n_examples)
      );
    }
    BOOST_CHECK_EQUAL(
      r.SuggestConcreteness(n_examples),
      RatingConcreteness::SuggestConcretenessDefault(n_examples)
    );
    BOOST_CHECK_EQUAL(
      r.SuggestSpecificity(n_examples),
      RatingSpecificity::SuggestSpecificityDefault(n_examples)
    );
  }
}
