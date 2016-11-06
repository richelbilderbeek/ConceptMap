#ifndef CONCEPTMAPEDGEFACTORY_H
#define CONCEPTMAPEDGEFACTORY_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapfwd.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct EdgeFactory
{
  EdgeFactory() noexcept;

  Edge Create(
    const Node& from,
    const Node& to
  ) const noexcept;

  Edge Create(
    const Node& node
  ) const noexcept;

  int GetNumberOfTests() const noexcept;
  Edge GetTest(const int index) const noexcept;
  std::vector<Edge> GetTests() const noexcept;

  int GetNumberOfNastyTests() const noexcept;
  Edge GetNastyTest(const int index) const noexcept;
  std::vector<Edge> GetNastyTests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGEFACTORY_H
