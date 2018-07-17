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
  ) const;

  ///Calculate a suggested complexity
  int SuggestSpecificity(
    int n_examples
  ) const;

  private:
  std::map<int, int> m_rating;

  ///Calculate a suggested specificity
  static int SuggestSpecificityDefault(const int n_examples); //!OCLINT static because it needs no member variables

  friend bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
  friend std::string ToXml(const RatingSpecificity& rating);
};

RatingSpecificity CreateDefaultRatingSpecificity() noexcept;
RatingSpecificity CreateTestRatingSpecificity() noexcept;

std::string ToXml(const RatingSpecificity& rating);

///Extract a RatingSpecificity from an XML string
RatingSpecificity XmlToRatingSpecificity(const std::string& s);

bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
bool operator!=(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingSpecificity& r) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPRATINGSPECIFICITY_H