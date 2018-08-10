#ifndef CONCEPTMAPRATINGCONCRETENESS_H
#define CONCEPTMAPRATINGCONCRETENESS_H

#include <map>
#include "conceptmapfwd.h"

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
    int n_examples
  ) const;

  ///Calculate a suggested concreteness in the default way
  static int SuggestConcretenessDefault(const int n_examples); //!OCLINT static because it needs no member variables

  private:
  std::map<int, int> m_rating;


  friend bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
  friend std::string ToXml(const RatingConcreteness& rating);
};

RatingConcreteness CreateDefaultRatingConcreteness() noexcept;
RatingConcreteness CreateTestRatingConcreteness() noexcept;

///Extract a RatingConcreteness from an XML string
RatingConcreteness XmlToRatingConcreteness(const std::string& s);

///To be used as a tooltip
///@param rating a RatingConcreteness
///@param n_examples_emph the number of complex examples that will
///   be emphasized. No column will be highlighted
///   as such for invalid values
std::string ToHtml(
  const RatingConcreteness& rating,
  const int n_examples_emph = -1
);

std::string ToXml(const RatingConcreteness& rating);

bool operator==(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
bool operator!=(const RatingConcreteness& lhs, const RatingConcreteness& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingConcreteness& r) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPRATINGCONCRETENESS_H
