#ifndef CONCEPTMAPRATINGSPECIFICITY_H
#define CONCEPTMAPRATINGSPECIFICITY_H

#include <map>
#include "conceptmapfwd.h"

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
    int n_examples
  ) const;

  ///Calculate a suggested specificity in the default way
  static int SuggestSpecificityDefault(const int n_examples); //!OCLINT static because it needs no member variables

  private:
  std::map<int, int> m_rating;

  friend bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
  friend std::string ToXml(const RatingSpecificity& rating);
};

RatingSpecificity CreateDefaultRatingSpecificity() noexcept;
RatingSpecificity CreateTestRatingSpecificity() noexcept;

///To be used as a tooltip
///@param rating a RatingSpecificity
///@param n_examples_emph the number of complex examples that will
///   be emphasized. No column will be highlighted
///   as such for invalid values
std::string ToHtml(
  const RatingSpecificity& rating,
  const int n_examples_emph = -1
);

std::string ToXml(const RatingSpecificity& rating);

///Extract a RatingSpecificity from an XML string
RatingSpecificity XmlToRatingSpecificity(const std::string& s);

bool operator==(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
bool operator!=(const RatingSpecificity& lhs, const RatingSpecificity& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingSpecificity& r) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPRATINGSPECIFICITY_H
