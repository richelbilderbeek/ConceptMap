#include "conceptmapfactory.h"

#include "conceptmap.h"
#include "conceptmaphelper.h"

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

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_lonely_center_node)
{
  const auto concept_map = ConceptMapFactory().GetLonelyQtCenterNode();
  BOOST_CHECK_EQUAL(1, CountCenterNodes(concept_map));
  BOOST_CHECK_EQUAL(1, boost::num_vertices(concept_map));
  BOOST_CHECK_EQUAL(0, boost::num_edges(concept_map));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_star_shaped)
{
  const auto concept_map = ConceptMapFactory().GetStarShaped();
  BOOST_CHECK_EQUAL(1, CountCenterNodes(concept_map));
  BOOST_CHECK_EQUAL(5, boost::num_vertices(concept_map));
  BOOST_CHECK_EQUAL(3, boost::num_edges(concept_map));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_two_node_one_edge)
{
  const auto concept_map = ConceptMapFactory().GetTwoNodeOneEdge();
  BOOST_CHECK_EQUAL(1, CountCenterNodes(concept_map));
  BOOST_CHECK_EQUAL(2, boost::num_vertices(concept_map));
  BOOST_CHECK_EQUAL(1, boost::num_edges(concept_map));
}

BOOST_AUTO_TEST_CASE(
  ribi_concept_map_concept_factory_two_node_one_edge_no_center
)
{
  const auto concept_map = ConceptMapFactory().GetTwoNodeOneEdgeNoCenter();
  BOOST_CHECK_EQUAL(0, CountCenterNodes(concept_map));
  BOOST_CHECK_EQUAL(2, boost::num_vertices(concept_map));
  BOOST_CHECK_EQUAL(1, boost::num_edges(concept_map));
}

BOOST_AUTO_TEST_CASE(test_ribi_conceptmapfactory_others
) {
  BOOST_CHECK_NO_THROW(
    ConceptMapFactory().GetWithExamplesWithCompetencies(
      {
        ribi::cmap::Competency::organisations,
        ribi::cmap::Competency::profession
      }
    )
  );

  BOOST_CHECK_NO_THROW(
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  );

  BOOST_CHECK_NO_THROW(
    ConceptMapFactory().GetRateConceptTallyDialogExample283()
  );

  BOOST_CHECK_NO_THROW(
    ConceptMapFactory().GetUnrated()
  );
}
