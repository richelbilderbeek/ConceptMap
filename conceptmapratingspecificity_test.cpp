#include "conceptmapratingspecificity.h"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_comparison)
{
  const auto r = CreateDefaultRatingSpecificity();
  const auto s = CreateTestRatingSpecificity();
  BOOST_CHECK_EQUAL(r, r);
  BOOST_CHECK_EQUAL(s, s);
  BOOST_CHECK_NE(r, s);
  BOOST_CHECK_NE(s, r);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_to_html)
{
  const auto r = CreateDefaultRatingSpecificity();
  const auto html = ToHtml(r);
  BOOST_CHECK(html.find("<table") != std::string::npos);
  BOOST_CHECK(html.find("</table>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_to_html_differs)
{
  const auto r = CreateDefaultRatingSpecificity();
  const auto s = CreateTestRatingSpecificity();
  const auto html_1 = ToHtml(r);
  const auto html_2 = ToHtml(s);
  BOOST_CHECK_NE(r, s);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_to_html_examples_emph)
{
  const auto r = CreateDefaultRatingSpecificity();
  //                            n_examples_emph
  const auto html_1 = ToHtml(r, 1);
  const auto html_2 = ToHtml(r, 2);
  BOOST_CHECK_EQUAL(html_1, html_1);
  BOOST_CHECK_NE(html_1, html_2);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_rating_specificity_to_xml)
{
  const auto r = CreateDefaultRatingSpecificity();
  const auto s = CreateTestRatingSpecificity();
  const auto xml_1 = ToXml(r);
  const auto xml_2 = ToXml(s);
  BOOST_CHECK_NE(xml_1, xml_2);
}
