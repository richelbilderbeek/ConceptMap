#ifndef CONCEPTMAPCONCEPTFACTORY_H
#define CONCEPTMAPCONCEPTFACTORY_H

#include <array>
#include <string>
#include <vector>
#include "conceptmapcompetency.h"
#include "conceptmapfwd.h"

namespace ribi {
namespace cmap {

///Creates Concepts
struct ConceptFactory
{
  ConceptFactory() noexcept;

  Concept Create(
    const std::string& name = "...",
    const std::vector<std::pair<std::string,Competency> >& examples = {},
    const bool is_complex = true,
    const int rating_complexity = -1,
    const int rating_concreteness = -1,
    const int rating_specificity = -1
  ) const noexcept;

  Concept GetTest(const int i) const noexcept;

  Concept Get0() const noexcept;
  Concept Get1() const noexcept;
  Concept Get2() const noexcept;

  Concept GetNasty0() const noexcept;
  Concept GetNasty1() const noexcept;
  Concept GetNasty2() const noexcept;

  std::vector<Concept> GetNastyTests() const noexcept;
  std::vector<Concept> GetTests() const noexcept;
  int GetNumberOfTests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTFACTORY_H
