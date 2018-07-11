#ifndef CONCEPTMAPREGEX_H
#define CONCEPTMAPREGEX_H

#include <string>
#include <vector>







namespace ribi {
namespace cmap {

///Factory for creating Node instances
struct Regex
{
  Regex();

  std::string GetRegexCompetency() const noexcept;
  std::string GetRegexComplexity() const noexcept;
  std::string GetRegexConcept() const noexcept;
  std::string GetRegexConceptMap() const noexcept;
  std::string GetRegexConceptIsComplex() const noexcept;
  std::string GetRegexConcreteness() const noexcept;
  std::string GetRegexExample() const noexcept;
  std::string GetRegexExamples() const noexcept;
  std::string GetRegexHasHead() const noexcept;
  std::string GetRegexHasTail() const noexcept;
  std::string GetRegexIsCenterNode() const noexcept;
  std::string GetRegexIsComplex() const noexcept;
  std::string GetRegexIsConcrete() const noexcept;
  std::string GetRegexIsSpecific() const noexcept;
  std::string GetRegexName() const noexcept;
  std::string GetRegexSpecificity() const noexcept;
  std::string GetRegexText() const noexcept;
  std::string GetRegexX() const noexcept;
  std::string GetRegexY() const noexcept;

  std::vector<std::string>
    GetRegexMatches(
    const std::string& s,
    const std::string& regex_str
  ) const noexcept;
};

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPREGEX_H
