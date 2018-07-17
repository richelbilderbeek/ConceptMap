#ifndef CONCEPTMAPRATINGSPECIFICITY_H
#define CONCEPTMAPRATINGSPECIFICITY_H

#include <map>
#include "conceptmapfwd.h"
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

class RatingSpecificity
{
  public:

  RatingSpecificity(
    const std::map<int, int>& rating
  );

  ///Calculate a suggested complexity
  int SuggestSpecificity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  private:
  std::map<int, int> m_rating;

  friend bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
};

RatingSpecificity CreateDefaultRatingSpecificity() noexcept;
RatingSpecificity CreateTestRatingSpecificity() noexcept;

///Calculate a suggested specificity
int SuggestSpecificity(const int n_examples) noexcept;

std::string ToXml(const RatingSpecificity& rating);

bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
bool operator!=(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingSpecificity& r) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPRATINGSPECIFICITY_H
