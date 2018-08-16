#include "conceptmapnodetype.h"

#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(ribi_cmap_to_node_type)
{
  BOOST_CHECK_EQUAL(
    ToNodeType("normal"), NodeType::normal
  );
  BOOST_CHECK_EQUAL(
    ToNodeType("center"), NodeType::center
  );
  BOOST_CHECK_THROW(
    ToNodeType("nonsense"), std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_type_to_str)
{
  BOOST_CHECK_EQUAL(
    "normal", ToStr(NodeType::normal)
  );
  BOOST_CHECK_EQUAL(
    "center", ToStr(NodeType::center)
  );
}
