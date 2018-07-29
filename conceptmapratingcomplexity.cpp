#include "conceptmapratingcomplexity.h"
#include "ribi_regex.h"
#include "xml.h"

ribi::cmap::RatingComplexity::RatingComplexity(
  const std::map<std::pair<int, int>, int> &rating_complexity
) : m_rating{rating_complexity}
{
  // [0, 3] number of nodes, [0, 3] number of examples
  assert(m_rating.size() == 16);
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
      { {1, 0}, 0 },
      { {1, 1}, 1 },
      { {1, 2}, 1 },
      { {1, 3}, 1 },
      { {2, 0}, 1 },
      { {2, 1}, 2 },
      { {2, 2}, 2 },
      { {2, 3}, 2 },
      { {3, 0}, 2 },
      { {3, 1}, 2 },
      { {3, 2}, 2 },
      { {3, 3}, 2 }
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
      { {1, 0}, 1 },
      { {1, 1}, 2 },
      { {1, 2}, 2 },
      { {1, 3}, 2 },
      { {2, 0}, 2 },
      { {2, 1}, 0 },
      { {2, 2}, 0 },
      { {2, 3}, 0 },
      { {3, 0}, 0 },
      { {3, 1}, 0 },
      { {3, 2}, 0 },
      { {3, 3}, 0 }
    }
  );
}

int ribi::cmap::RatingComplexity::SuggestComplexityDefault( //!OCLINT static because it needs no member variables
  const int n_edges,
  const int n_examples
)
{
  assert(n_edges >= 0);
  assert(n_examples >= 0);
  return n_edges == 0  || (n_edges == 1 && n_examples == 0)
    ? 0
    : (n_edges == 1 && n_examples > 0) || (n_edges == 2 && n_examples == 0)
      ? 1
      : 2
  ;
}

int ribi::cmap::RatingComplexity::SuggestComplexity(
  const int n_edges,
  const int n_examples
) const
{
  const auto iter = m_rating.find( { n_edges, n_examples} );
  assert(iter != std::end(m_rating));
  return iter->second;
}

int ribi::cmap::RatingComplexity::SuggestComplexity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const
{
  const int n_edges = std::min(3, static_cast<int>(boost::num_edges(sub_conceptmap)));
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = std::min(4, CountExamples(sub_conceptmap[vd]));
  return SuggestComplexity(n_edges, n_examples);
}

std::string ribi::cmap::ToXml(const RatingComplexity& rating)
{
  std::stringstream s;
  s << "<rating_complexity>";
  for (const auto& p: rating.m_rating)
  {
    assert(p.first.first  >= 0);
    assert(p.first.second  >= 0);
    assert(p.second >= 0);
    assert(p.first.first  <= 9);
    assert(p.first.second  <= 9);
    assert(p.second <= 9);
    s << p.first.first << p.first.second << p.second;
  }
  s << "</rating_complexity>";
  const std::string r = s.str();
  assert(r.size() >= 39);
  assert(r.substr(0, 19) == "<rating_complexity>");
  assert(r.substr(r.size() - 20, 20) == "</rating_complexity>");
  return r;
}

ribi::cmap::RatingComplexity ribi::cmap::XmlToRatingComplexity(
  const std::string& s)
{
  const std::string regex_str = "(<rating_complexity>.*?</rating_complexity>)";
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s, regex_str);
  assert(v.size() == 1);

  const std::string t = ribi::xml::StripXmlTag(v[0]);
  assert(t.size() % 3 == 0);
  const int size = t.size();

  std::map<std::pair<int, int>, int> m;
  for (int i = 0; i != size; i += 3)
  {
    const int first = boost::lexical_cast<int>(t[i]);
    const int second = boost::lexical_cast<int>(t[i + 1]);
    const int third = boost::lexical_cast<int>(t[i + 2]);
    assert(first >= 0);
    assert(second >= 0);
    assert(third >= 0);
    assert(first <= 9);
    assert(second <= 9);
    assert(third <= 9);
    m.insert( { { first, second }, third } );
  }
  return RatingComplexity(m);
}

bool ribi::cmap::operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept
{
  return lhs.m_rating == rhs.m_rating;
}

bool ribi::cmap::operator!=(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const RatingComplexity& r) noexcept
{
  os << ToXml(r);
  return os;
}
