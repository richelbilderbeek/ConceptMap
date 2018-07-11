#ifndef CONCEPTMAPEXAMPLEFACTORY_H
#define CONCEPTMAPEXAMPLEFACTORY_H

#include <vector>




#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"


namespace ribi {
namespace cmap {

struct ExampleFactory
{
  ExampleFactory() noexcept;

  int GetNumberOfTests() const noexcept { return static_cast<int>(GetTests().size()); }
  Example GetTest(const int i) const noexcept;
  std::vector<Example> GetTests() const noexcept;
  std::vector<Example> GetNastyTests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEXAMPLEFACTORY_H
