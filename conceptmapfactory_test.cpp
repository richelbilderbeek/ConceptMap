#include "conceptmapfactory.h"

#include "conceptmap.h"

#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_test_concept_maps_must_have_one_center_node)
{
  const ConceptMapFactory f;
  for (const auto& c: f.GetAllTests())
  {
    if (boost::num_vertices(c) == 0) continue;
    BOOST_CHECK_EQUAL(CountCenterNodes(c), 1);
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_test_concept_maps_must_have_one_center_node_without_examples)
{
  const ConceptMapFactory f;
  for (const auto& c: f.GetAllTests())
  {
    if (boost::num_vertices(c) == 0) continue;
    BOOST_CHECK(GetCenterNode(c).GetConcept().GetExamples().Get().empty());
  }
}

BOOST_AUTO_TEST_CASE(both_edges_of_GetThreeNodeTwoEdgeNoCenter_have_examples)
{
  const ConceptMapFactory f;
  const ConceptMap m = f.GetThreeNodeTwoEdgeNoCenter();
  const auto edges = GetEdges(m);
  assert(edges.size() == 2);
  BOOST_CHECK(HasExamples(edges[0]));
  BOOST_CHECK(HasExamples(edges[1]));
}
