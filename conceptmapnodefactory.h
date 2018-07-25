#ifndef CONCEPTMAPNODEFACTORY_H
#define CONCEPTMAPNODEFACTORY_H

#include <vector>




#include "conceptmapcompetency.h"
#include "conceptmapfwd.h"


namespace ribi {
namespace cmap {

struct Node;

///Factory for creating Node instances
struct NodeFactory
{
  NodeFactory();

  Node CreateFromStrings(
    const std::string& name,
    const std::vector<std::string>& examples = {},
    const double x = 0.0,
    const double y = 0.0
  ) const noexcept;


  int GetNumberOfTests() const noexcept;
  std::vector<Node> GetTests() const noexcept;
  Node GetTest(const int test) const;

  int GetNumberOfNastyTests() const noexcept;
  std::vector<Node> GetNastyTests() const noexcept;
  Node GetNasty0() const noexcept;
  Node GetNasty1() const noexcept;
  Node GetNasty2() const noexcept;
  Node GetNastyTest(const int test) const noexcept;

  private:
  Node CreateFromStringsOld(
    const std::string& name,
    const std::vector<std::pair<std::string,Competency> >& examples = {},
    const double x = 0.0,
    const double y = 0.0
  ) const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPNODEFACTORY_H
