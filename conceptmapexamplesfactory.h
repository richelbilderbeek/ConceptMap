#ifndef CONCEPTMAPEXAMPLESFACTORY_H
#define CONCEPTMAPEXAMPLESFACTORY_H

#include <vector>




#include "conceptmapcompetency.h"
#include "conceptmapexamples.h"
#include "conceptmapfwd.h"


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
