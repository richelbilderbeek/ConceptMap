#ifndef CONCEPTMAPRATINGCONCRETENESS_H
#define CONCEPTMAPRATINGCONCRETENESS_H

#include <map>
#include "conceptmapfwd.h"
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

class RatingConcreteness
{
  public:

  RatingConcreteness(
    const std::map<int, int>& rating
  );

  ///Calculate a suggested complexity
  int SuggestConcreteness(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  private:
  std::map<int, int> m_rating;

  friend bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
};

RatingConcreteness CreateDefaultRatingConcreteness() noexcept;
RatingConcreteness CreateTestRatingConcreteness() noexcept;

///Calculate a suggested concreteness
int SuggestConcreteness(const int n_examples) noexcept;

std::string ToXml(const RatingConcreteness& rating);

bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
bool operator!=(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingConcreteness& r) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPRATINGCONCRETENESS_H
