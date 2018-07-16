#include "conceptmapratingcomplexity.h"

ribi::cmap::RatingComplexity::RatingComplexity(
  const std::map<std::pair<int, int>, int> &rating_complexity
) : m_rating_complexity{rating_complexity}
{
  // [0, 3] number of nodes, [0, 4] number of examples
  assert(m_rating_complexity.size() == 20);
}

ribi::cmap::RatingComplexity ribi::cmap::CreateDefaultRatingComplexity() noexcept
{
  // { {number of edges, number of examples}, score }
  return RatingComplexity(
    {
      { {0, 0}, 0 },
      { {0, 1}, 0 },
      { {0, 2}, 0 },
      { {0, 3}, 0 },
      { {0, 4}, 0 },
      { {1, 0}, 0 },
      { {1, 1}, 1 },
      { {1, 2}, 1 },
      { {1, 3}, 1 },
      { {1, 4}, 1 },
      { {2, 0}, 1 },
      { {2, 1}, 2 },
      { {2, 2}, 2 },
      { {2, 3}, 2 },
      { {2, 4}, 2 },
      { {3, 0}, 2 },
      { {3, 1}, 2 },
      { {3, 2}, 2 },
      { {3, 3}, 2 },
      { {3, 4}, 2 }
    }
  );
}

ribi::cmap::RatingComplexity ribi::cmap::CreateTestRatingComplexity() noexcept
{
  // { {number of edges, number of examples}, score }
  return RatingComplexity(
    {
      { {0, 0}, 1 },
      { {0, 1}, 1 },
      { {0, 2}, 1 },
      { {0, 3}, 1 },
      { {0, 4}, 1 },
      { {1, 0}, 1 },
      { {1, 1}, 2 },
      { {1, 2}, 2 },
      { {1, 3}, 2 },
      { {1, 4}, 2 },
      { {2, 0}, 2 },
      { {2, 1}, 0 },
      { {2, 2}, 0 },
      { {2, 3}, 0 },
      { {2, 4}, 0 },
      { {3, 0}, 0 },
      { {3, 1}, 0 },
      { {3, 2}, 0 },
      { {3, 3}, 0 },
      { {3, 4}, 0 }
    }
  );
}

int ribi::cmap::SuggestComplexity(
  const int n_edges,
  const int n_examples
) noexcept
{
  return n_edges == 0  || (n_edges == 1 && n_examples == 0)
    ? 0
    : (n_edges == 1 && n_examples > 0) || (n_edges == 2 && n_examples == 0)
      ? 1
      : 2
  ;
}

int ribi::cmap::RatingComplexity::SuggestComplexity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  const int n_edges = std::min(3, static_cast<int>(boost::num_edges(sub_conceptmap)));
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = std::min(4, CountExamples(sub_conceptmap[vd]));
  const auto iter = m_rating_complexity.find( { n_edges, n_examples} );
  assert(iter != std::end(m_rating_complexity));
  assert(iter->second == ::ribi::cmap::SuggestComplexity(n_edges, n_examples));
  return iter->second;
}

bool ribi::cmap::operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept
{
  return lhs.m_rating_complexity == rhs.m_rating_complexity;
}

bool ribi::cmap::operator!=(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const RatingComplexity& ) noexcept
{
  os << "TODO";
  return os;
}
