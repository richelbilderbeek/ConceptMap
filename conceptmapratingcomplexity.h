#ifndef CONCEPTMAPRATINGCOMPLEXITY_H
#define CONCEPTMAPRATINGCOMPLEXITY_H

#include <map>
#include "conceptmapfwd.h"
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

class RatingComplexity
{
  public:

  RatingComplexity(
    const std::map<std::pair<int, int>, int>& rating_complexity
  );

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  private:
  std::map<std::pair<int, int>, int> m_rating;

  friend bool operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
  friend std::string ToXml(const RatingComplexity& rating);
};

RatingComplexity CreateDefaultRatingComplexity() noexcept;

RatingComplexity CreateTestRatingComplexity() noexcept;

///Exract a RatingComplexity from an XML string
RatingComplexity XmlToRatingComplexity(const std::string& s);

///Calculate a suggested complexity
int SuggestComplexity(const int n_edges, const int n_examples) noexcept;

std::string ToXml(const RatingComplexity& rating);

bool operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
bool operator!=(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingComplexity& r) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPRATINGCOMPLEXITY_H
