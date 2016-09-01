#include "conceptmapedgefactory.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapedge.h"
#include "conceptmapnodefactory.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_factory_test)
{
  const ribi::cmap::EdgeFactory f;
  BOOST_CHECK(!f.GetNastyTests().empty());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_factory_create)
{
  using namespace ribi::cmap;
  EdgeFactory f;
  const auto from = NodeFactory().GetNasty0();
  const auto to = NodeFactory().GetNasty0();

  BOOST_CHECK_NO_THROW(
    f.Create(from, to)
  );

  BOOST_CHECK_NO_THROW(
    f.Create(from)
  );
}
