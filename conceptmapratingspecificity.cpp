#include "conceptmapratingspecificity.h"

#include <boost/lexical_cast.hpp>

#include "conceptmapratingconcreteness.h"
#include "ribi_regex.h"
#include "xml.h"

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
      {4, 2}
    }
  );
}

int ribi::cmap::RatingSpecificity::SuggestSpecificityDefault(const int n_examples) //!OCLINT static because it needs no member variables
{
  assert(n_examples >= 0);
  return n_examples < 2
    ? 0
    : n_examples > 1 && n_examples < 4
      ? 1
      : 2
  ;
}

int ribi::cmap::RatingSpecificity::SuggestSpecificity(
  const int n_examples
) const
{
  const auto iter = m_rating.find(n_examples);
  if (iter == std::end(m_rating))
  {
    if (n_examples == 0) return 0;
    return SuggestSpecificity(n_examples - 1);
  }
  return iter->second;
}

std::string ribi::cmap::ToHtml(
  const RatingSpecificity& r,
  const int n_examples_emph
)
{
  std::stringstream s;
  s << "<table style=\"vertical-align: middle\">\n"
    << "  <tr>\n"
    << "    <th> </th><th colspan=\"5\">Aantal concrete</th>\n"
    << "  </tr>\n"
    << "  <tr>\n"
    << "    <th> </th><th colspan=\"5\">voorbeelden</th>\n"
    << "  </tr>\n"
    << "  <tr>\n"
    << "    <th> </th>"
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
  s << "  <tr>\n";
  s << "    <th><nobr>Voorgestelde specificiteit</nobr></th>\n";
  for (int n_examples = 0; n_examples != 5; ++n_examples)
  {
    s << "    <td><center>";
    const bool emphasized{
      n_examples == n_examples_emph
    };
    if (emphasized) { s << "<u>"; }
    s << r.SuggestSpecificity(n_examples);
    if (emphasized) { s << "</u>"; }
    s << "</center></td>\n";
  }
  s << "  </tr>\n";
  s << "</table>";
  return s.str();
}

std::string ribi::cmap::ToXml(const RatingSpecificity& rating)
{
  std::stringstream s;
  s << "<rating_specificity>";
  for (const auto& p: rating.m_rating)
  {
    assert(p.first  >= 0);
    assert(p.second >= 0);
    assert(p.first  <= 9);
    assert(p.second <= 9);
    s << p.first << p.second;
  }
  s << "</rating_specificity>";
  const std::string r = s.str();
  assert(r.size() >= 41);
  assert(r.substr(0, 20) == "<rating_specificity>");
  assert(r.substr(r.size() - 21, 21) == "</rating_specificity>");
  return r;
}

ribi::cmap::RatingSpecificity ribi::cmap::XmlToRatingSpecificity(
  const std::string& s)
{
  const std::string regex_str = "(<rating_specificity>.*?</rating_specificity>)";
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s, regex_str);
  assert(v.size() == 1);

  const std::string t = ribi::xml::StripXmlTag(v[0]);
  assert(t.size() % 2 == 0);
  const int size = t.size();

  std::map<int, int> m;
  for (int i = 0; i != size; i += 2)
  {
    const int first = boost::lexical_cast<int>(t[i]);
    const int second = boost::lexical_cast<int>(t[i + 1]);
    assert(first >= 0);
    assert(second >= 0);
    assert(first <= 9);
    assert(second <= 9);
    m.insert( { first, second } );
  }
  return RatingSpecificity(m);
}

bool ribi::cmap::operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept
{
  return lhs.m_rating == rhs.m_rating;
}

bool ribi::cmap::operator!=(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const RatingSpecificity& r) noexcept
{
  os << ToXml(r);
  return os;
}
