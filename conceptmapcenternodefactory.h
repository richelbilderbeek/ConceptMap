#ifndef CONCEPTMAPCENTERNODEFACTORY_H
#define CONCEPTMAPCENTERNODEFACTORY_H

#include <string>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapcompetency.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct CenterNodeFactory
{
  CenterNodeFactory();

  Node Create() const noexcept;

  Node Create(
    const Concept& concept,
    const double x = 0.0,
    const double y = 0.0
  ) const noexcept;

  Node CreateFromStrings(
    const std::string& name,
    const std::vector<std::pair<std::string,Competency> >& examples = {},
    const double x = 0.0,
    const double y = 0.0
  ) const noexcept;

  int GetNumberOfTests() const noexcept;
  std::vector<Node> GetTests() const noexcept;
  Node GetTest(const int test_index) const noexcept;

  int GetNumberOfNastyTests() const noexcept;
  Node GetNasty0() const noexcept;
  std::vector<Node> GetNastyTests() const noexcept;
  Node GetNastyTest(const int test_index) const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCENTERNODEFACTORY_H
