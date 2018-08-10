#include "conceptmaprating.h"

#include <cassert>
#include <cmath>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"

ribi::cmap::Rating::Rating(
  const RatingComplexity& rating_complexity,
  const RatingConcreteness& rating_concreteness,
  const RatingSpecificity& rating_specificity
)
  : m_rating_complexity{rating_complexity},
    m_rating_concreteness{rating_concreteness},
    m_rating_specificity{rating_specificity}
{
}

ribi::cmap::Rating ribi::cmap::CreateDefaultRating() noexcept
{
  return Rating(
    CreateDefaultRatingComplexity(),
    CreateDefaultRatingConcreteness(),
    CreateDefaultRatingSpecificity()
  );
}

ribi::cmap::Rating ribi::cmap::CreateTestRating() noexcept
{
  return Rating(
    CreateTestRatingComplexity(),
    CreateTestRatingConcreteness(),
    CreateTestRatingSpecificity()
  );
}

int ribi::cmap::Rating::SuggestComplexity(
  const int n_edges,
  const int n_examples
) const noexcept
{
  return m_rating_complexity.SuggestComplexity(n_edges, n_examples);
}



int ribi::cmap::Rating::SuggestConcreteness(
  const int n_examples
) const noexcept
{
  return m_rating_concreteness.SuggestConcreteness(n_examples);
}

int ribi::cmap::Rating::SuggestSpecificity(
  const int n_examples
) const noexcept
{
  return m_rating_specificity.SuggestSpecificity(n_examples);
}

std::string ribi::cmap::ToXml(const Rating& rating)
{
  std::stringstream s;
  s << "<rating>"
    << ToXml(rating.m_rating_complexity)
    << ToXml(rating.m_rating_concreteness)
    << ToXml(rating.m_rating_specificity)
    << "</rating>"
  ;
  const std::string r = s.str();
  assert(r.size() >= 17);
  assert(r.substr(0, 8) == "<rating>");
  assert(r.substr(r.size() - 9, 9) == "</rating>");
  return r;
}

ribi::cmap::Rating ribi::cmap::XmlToRating(const std::string& s)
{
  return Rating(
    XmlToRatingComplexity(s),
    XmlToRatingConcreteness(s),
    XmlToRatingSpecificity(s)
  );
}


bool ribi::cmap::operator==(const Rating& lhs, const Rating& rhs) noexcept
{
  return lhs.m_rating_complexity == rhs.m_rating_complexity
    && lhs.m_rating_concreteness == rhs.m_rating_concreteness
    && lhs.m_rating_specificity == rhs.m_rating_specificity
  ;
}

bool ribi::cmap::operator!=(const Rating& lhs, const Rating& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Rating& r) noexcept
{
  os << ToXml(r);
  return os;
}
