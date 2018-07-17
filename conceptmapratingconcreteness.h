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
  ) const;

  ///Calculate a suggested complexity
  int SuggestConcreteness(
    int n_examples
  ) const;

  private:
  std::map<int, int> m_rating;

  ///Calculate a suggested concreteness
  static int SuggestConcretenessDefault(const int n_examples); //!OCLINT static because it needs no member variables

  friend bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
  friend std::string ToXml(const RatingConcreteness& rating);
};

RatingConcreteness CreateDefaultRatingConcreteness() noexcept;
RatingConcreteness CreateTestRatingConcreteness() noexcept;

///Extract a RatingConcreteness from an XML string
RatingConcreteness XmlToRatingConcreteness(const std::string& s);


std::string ToXml(const RatingConcreteness& rating);

bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
bool operator!=(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingConcreteness& r) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPRATINGCONCRETENESS_H