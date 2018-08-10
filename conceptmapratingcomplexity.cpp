#include "conceptmapratingcomplexity.h"

#include <boost/lexical_cast.hpp>

#include "ribi_regex.h"
#include "xml.h"

ribi::cmap::RatingComplexity::RatingComplexity(
  const std::map<std::pair<int, int>, int> &rating_complexity
) : m_rating{rating_complexity}
{
  // [0, 3] (that is 4) number of relations,
  // [0, 4] (that is 5) number of examples
  assert(m_rating.size() == 20);
}

ribi::cmap::RatingComplexity ribi::cmap::CreateDefaultRatingComplexity() noexcept
{
  // { {number of edges, number of examples}, score }
  return RatingComplexity(
    {
      { {0, 0}, 0 },
      { {0, 1}, 0 },
      { {0, 2}, 1 },
      { {0, 3}, 1 },
      { {0, 4}, 2 },
      { {1, 0}, 0 },
      { {1, 1}, 1 },
      { {1, 2}, 1 },
      { {1, 3}, 2 },
      { {1, 4}, 2 },
      { {2, 0}, 1 },
      { {2, 1}, 1 },
      { {2, 2}, 2 },
      { {2, 3}, 2 },
      { {2, 4}, 2 },
      { {3, 0}, 1 },
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

int ribi::cmap::RatingComplexity::SuggestComplexityDefault( //!OCLINT static because it needs no member variables
  const int n_edges,
  const int n_examples
)
{
  assert(n_edges >= 0);
  assert(n_examples >= 0);
  if (n_edges == 0)
  {
    if (n_examples == 0) return 0;
    if (n_examples == 1) return 0;
    if (n_examples == 2) return 1;
    if (n_examples == 3) return 1;
    assert(n_examples  > 3);
    return 2;
  }
  if (n_edges == 1)
  {
    if (n_examples == 0) return 0;
    if (n_examples == 1) return 1;
    if (n_examples == 2) return 1;
    assert(n_examples  >= 3);
    return 2;
  }
  if (n_edges == 2)
  {
    if (n_examples == 0) return 1;
    if (n_examples == 1) return 1;
    assert(n_examples >= 2);
    return 2;
  }
  if (n_edges == 3)
  {
    if (n_examples == 0) return 1;
    assert(n_examples >= 1);
    return 2;
  }
  assert(n_edges > 3);
  return 2;
}

int ribi::cmap::RatingComplexity::SuggestComplexity(
  const int n_edges,
  const int n_examples
) const
{
  assert(n_edges >= 0);
  assert(n_examples >= 0);
  const auto iter = m_rating.find( { n_edges, n_examples} );
  if (iter == std::end(m_rating))
  {
    return 2;
  }
  return iter->second;
}

int ribi::cmap::RatingComplexity::SuggestComplexity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const
{
  //BUG
  const int n_edges = boost::num_edges(sub_conceptmap);
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = std::min(3, CountExamples(sub_conceptmap[vd]));
  return SuggestComplexity(n_edges, n_examples);
}

std::string ribi::cmap::ToHtml(
  const RatingComplexity& r,
  const int n_examples_emph,
  const int n_relations_emph
)
{
  std::stringstream s;
  s << "<table style=\"vertical-align: middle\">\n"
    << ToHtmlHeaderX(n_examples_emph)
  ;
  for (int n_relations = 0; n_relations != 4; ++n_relations)
  {
    s << "  <tr>\n";
    if (n_relations == 0)
    {
      s << "    <th rowspan=\"4\"><center>Aantal relaties</center></th>\n";
    }
    s << "    <th>";
    if (n_relations == n_relations_emph) { s << "<u>"; }
    s << n_relations;
    if (n_relations == n_relations_emph) { s << "</u>"; }
    s << "</th>\n";
    for (int n_examples = 0; n_examples != 5; ++n_examples)
    {
      s << "    <td><center>";
      const bool emphasized{
        n_relations == n_relations_emph
        || n_examples == n_examples_emph
      };
      if (emphasized) { s << "<u>"; }
      s << r.SuggestComplexity(n_relations, n_examples);
      if (emphasized) { s << "</u>"; }
      s << "</center></td>\n";
    }
    s << "  </tr>\n";
  }
  s
    << "</table>"
  ;
  return s.str();
}

std::string ribi::cmap::ToHtmlHeaderX(
  const int n_examples_emph
)
{
  std::stringstream s;
  s << "  <tr>\n"
    << "    <th>Voorgestelde</th><th> </th><th colspan=\"5\">Aantal complexe</th>\n"
    << "  </tr>\n"
    << "  <tr>\n"
    << "    <th>complexiteit</th><th> </th><th colspan=\"5\">relaties en voorbeelden</th>\n"
    << "  </tr>\n"
    << "  <tr>\n"
    << "    <th> </th><th> </th>"
  ;
    for (int n_examples = 0; n_examples != 4; ++n_examples)
    {
      s << "<th>";
      if (n_examples == n_examples_emph) { s << "<u>"; }
      s << n_examples;
      if (n_examples == n_examples_emph) { s << "</u>"; }
      s << "</th>";
    }
  s << "<th>&gt;3</th>\n";
  s << "  </tr>\n";
  return s.str();
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
