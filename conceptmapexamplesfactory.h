#ifndef CONCEPTMAPEXAMPLESFACTORY_H
#define CONCEPTMAPEXAMPLESFACTORY_H

#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapcompetency.h"
#include "conceptmapexamples.h"
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

struct ExamplesFactory
{
  ExamplesFactory() noexcept;

  Examples Create(
    const std::vector<std::pair<std::string,Competency> >& v) const noexcept;

  int GetNumberOfTests() const noexcept { return static_cast<int>(GetTests().size()); }
  Examples GetTest(const int i) const noexcept;
  Examples Get0() const noexcept;
  std::vector<Examples> GetNastyTests() const noexcept;
  std::vector<Examples> GetTests() const noexcept;
};

} //~namespace cmap

} //~namespace ribi

#endif // CONCEPTMAPEXAMPLESFACTORY_H
