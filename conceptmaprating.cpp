#include "conceptmaprating.h"

#include <cassert>
#include <cmath>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"

ribi::cmap::Rating::Rating(
  const RatingComplexity& rating_complexity,
  const std::map<int, int>& rating_concreteness,
  const std::map<int, int>& rating_specificity
)
  : m_rating_complexity{rating_complexity},
    m_rating_concreteness{rating_concreteness},
    m_rating_specificity{rating_specificity}
{
  // [0, 4] examples
  assert(m_rating_concreteness.size() == 5);
  // [0, 4] examples
  assert(m_rating_specificity.size() == 5);
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

std::map<int, int> ribi::cmap::CreateDefaultRatingConcreteness() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 0},
    {1, 0},
    {2, 1},
    {3, 1},
    {4, 2}
  };
}

std::map<int, int> ribi::cmap::CreateTestRatingConcreteness() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 1},
    {1, 1},
    {2, 2},
    {3, 2},
    {4, 0}
  };
}


std::map<int, int> ribi::cmap::CreateDefaultRatingSpecificity() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 0},
    {1, 0},
    {2, 1},
    {3, 1},
    {4, 2}
  };
}

std::map<int, int> ribi::cmap::CreateTestRatingSpecificity() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 1},
    {1, 1},
    {2, 2},
    {3, 2},
    {4, 0}
  };
}

int ribi::cmap::Rating::SuggestComplexity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  return m_rating_complexity.SuggestComplexity(sub_conceptmap, vd);
}


int ribi::cmap::SuggestConcreteness(const int n_examples) noexcept
{
  return n_examples < 2
    ? 0
    : n_examples > 1 && n_examples < 4
      ? 1
      : 2
  ;
}

int ribi::cmap::Rating::SuggestConcreteness(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);

  const auto iter = m_rating_concreteness.find(n_examples);
  assert(iter != std::end(m_rating_concreteness));
  assert(iter->second == ::ribi::cmap::SuggestConcreteness(n_examples));
  return iter->second;
}

int ribi::cmap::SuggestSpecificity(const int n_examples) noexcept
{
  return SuggestConcreteness(n_examples);
}

int ribi::cmap::Rating::SuggestSpecificity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);
  const auto iter = m_rating_specificity.find(n_examples);
  assert(iter != std::end(m_rating_specificity));
  assert(iter->second == ::ribi::cmap::SuggestSpecificity(n_examples));
  return iter->second;
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
  os << r.m_rating_complexity;
  /*
  std::copy(
    std::begin(r.m_rating_concreteness),
    std::end(r.m_rating_concreteness),
    std::ostream_iterator<std::string>(os)
  );

  std::copy(
    std::begin(r.m_rating_specificity),
    std::end(r.m_rating_specificity),
    std::ostream_iterator<std::string>(os)
  );
  */
  return os;
}
