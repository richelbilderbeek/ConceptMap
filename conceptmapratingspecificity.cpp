#include "conceptmapratingspecificity.h"
#include "conceptmapratingconcreteness.h"
ribi::cmap::RatingSpecificity::RatingSpecificity(
  const std::map<int, int>& rating
) : m_rating{rating}
{
  // [0, 4] examples
  assert(m_rating.size() == 5);
}

ribi::cmap::RatingSpecificity ribi::cmap::CreateDefaultRatingSpecificity() noexcept
{
  // { n_examples, score }
  return RatingSpecificity(
    {
      {0, 0},
      {1, 0},
      {2, 1},
      {3, 1},
      {4, 2}
    }
  );
}

ribi::cmap::RatingSpecificity ribi::cmap::CreateTestRatingSpecificity() noexcept
{
  // { n_examples, score }
  return RatingSpecificity(
    {
      {0, 1},
      {1, 1},
      {2, 2},
      {3, 2},
      {4, 0}
    }
  );
}

int ribi::cmap::SuggestSpecificity(const int n_examples) noexcept
{
  return ::ribi::cmap::SuggestConcreteness(n_examples);
}

int ribi::cmap::RatingSpecificity::SuggestSpecificity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);
  const auto iter = m_rating.find(n_examples);
  assert(iter != std::end(m_rating));
  assert(iter->second == ::ribi::cmap::SuggestSpecificity(n_examples));
  return iter->second;
}

std::string ribi::cmap::ToXml(const RatingSpecificity& /* rating */)
{
  std::stringstream s;
  s << "<rating_specificity>"
    << "TODO"
    << "</rating_specificity>"
  ;
  const std::string r = s.str();
  assert(r.size() >= 41);
  assert(r.substr(0, 20) == "<rating_specificity>");
  assert(r.substr(r.size() - 21, 21) == "</rating_specificity>");
  return r;
}

bool ribi::cmap::operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept
{
  return lhs.m_rating == rhs.m_rating;
}

bool ribi::cmap::operator!=(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const RatingSpecificity& /* r */) noexcept
{
  os << "TODO";
  return os;
}
