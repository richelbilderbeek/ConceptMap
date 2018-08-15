#include "conceptmapratingcomplexity.h"

#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_comparison)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto s = CreateTestRatingComplexity();
  BOOST_CHECK_EQUAL(r, r);
  BOOST_CHECK_EQUAL(s, s);
  BOOST_CHECK_NE(r, s);
  BOOST_CHECK_NE(s, r);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_html)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto html = ToHtml(r);
  BOOST_CHECK(html.find("<table") != std::string::npos);
  BOOST_CHECK(html.find("</table>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_html_differs)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto s = CreateTestRatingComplexity();
  assert(r != s);
  const auto html_1 = ToHtml(r);
  const auto html_2 = ToHtml(s);
  BOOST_CHECK_NE(r, s);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_html_examples_emph)
{
  const auto r = CreateDefaultRatingComplexity();
  //                            n_examples_emph
  const auto html_1 = ToHtml(r, 1);
  const auto html_2 = ToHtml(r, 2);
  BOOST_CHECK_EQUAL(html_1, html_1);
  BOOST_CHECK_NE(html_1, html_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_html_relations_emph)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto html_1 = ToHtml(r, 1, 1);
  const auto html_2 = ToHtml(r, 1, 2);
  BOOST_CHECK_EQUAL(html_1, html_1);
  BOOST_CHECK_NE(html_1, html_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_xml)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto s = CreateTestRatingComplexity();
  const auto xml_1 = ToXml(r);
  const auto xml_2 = ToXml(s);
  BOOST_CHECK_NE(xml_1, xml_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_xml_and_back)
{
  const auto r = CreateDefaultRatingComplexity();
  const auto xml = ToXml(r);
  const auto s = XmlToRatingComplexity(xml);
  BOOST_CHECK_EQUAL(r, s);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_complexity_to_stream)
{
  std::stringstream s;
  s << CreateDefaultRatingComplexity();
  BOOST_CHECK(!s.str().empty());
}
