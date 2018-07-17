#include "conceptmapratingconcreteness.h"

ribi::cmap::RatingConcreteness::RatingConcreteness(
  const std::map<int, int>& rating
) : m_rating{rating}
{
  // [0, 4] examples
  assert(m_rating.size() == 5);

}

ribi::cmap::RatingConcreteness ribi::cmap::CreateDefaultRatingConcreteness() noexcept
{
  // { n_examples, score }
  return RatingConcreteness(
    {
      {0, 0},
      {1, 0},
      {2, 1},
      {3, 1},
      {4, 2}
    }
  );
}

ribi::cmap::RatingConcreteness ribi::cmap::CreateTestRatingConcreteness() noexcept
{
  // { n_examples, score }
  return RatingConcreteness(
    {
      {0, 1},
      {1, 1},
      {2, 2},
      {3, 2},
      {4, 0}
    }
  );
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

int ribi::cmap::RatingConcreteness::SuggestConcreteness(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);

  const auto iter = m_rating.find(n_examples);
  assert(iter != std::end(m_rating));
  assert(iter->second == ::ribi::cmap::SuggestConcreteness(n_examples));
  return iter->second;
}

std::string ribi::cmap::ToXml(const RatingConcreteness& /* rating */)
{
  std::stringstream s;
  s << "<rating_concreteness>"
    << "TODO"
    << "</rating_concreteness>"
  ;
  const std::string r = s.str();
  assert(r.size() >= 43);
  assert(r.substr(0, 21) == "<rating_concreteness>");
  assert(r.substr(r.size() - 22, 22) == "</rating_concreteness>");
  return r;
}

bool ribi::cmap::operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept
{
  return lhs.m_rating == rhs.m_rating;
}

bool ribi::cmap::operator!=(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const RatingConcreteness& /* r */) noexcept
{
  os << "TODO";
  return os;
}
