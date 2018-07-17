#include "conceptmapratingspecificity.h"
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
