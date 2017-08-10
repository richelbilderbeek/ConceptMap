#include "conceptmapnodefactory.h"

#include <boost/test/unit_test.hpp>
#include "conceptmapnode.h"

BOOST_AUTO_TEST_CASE(node_factory_has_correct_number_of_tests)
{
  using namespace ribi::cmap;
  BOOST_CHECK(NodeFactory().GetNumberOfTests()
    == static_cast<int>(NodeFactory().GetTests().size())
  );
  BOOST_CHECK(NodeFactory().GetNumberOfNastyTests()
    == static_cast<int>(NodeFactory().GetNastyTests().size())
  );
}
