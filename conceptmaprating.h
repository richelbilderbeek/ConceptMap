#ifndef CONCEPTMAPRATING_H
#define CONCEPTMAPRATING_H

#include "conceptmapfwd.h"
#include "conceptmapratingcomplexity.h"
#include "conceptmapratingconcreteness.h"
#include "conceptmapratingspecificity.h"

namespace ribi {
namespace cmap {

///Functions to help with rating
struct Rating
{
  ///Use CreateDefaultRating for a default-constructed Rating
  Rating(
    const RatingComplexity& rating_complexity,
    const RatingConcreteness& rating_concreteness,
    const RatingSpecificity& rating_specificity
  );

  const auto& GetRatingComplexity() const noexcept { return m_rating_complexity; }
  const auto& GetRatingConcreteness() const noexcept { return m_rating_concreteness; }
  const auto& GetRatingSpecificity() const noexcept { return m_rating_specificity; }

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const int n_edges,
    const int n_examples
  ) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(
    int n_examples
  ) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(
    int n_examples
  ) const noexcept;

  private:
  RatingComplexity m_rating_complexity;
  RatingConcreteness m_rating_concreteness;
  RatingSpecificity m_rating_specificity;

  friend bool operator==(const Rating& lhs, const Rating& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const Rating& r) noexcept;
  friend std::string ToXml(const Rating& r);
};

///Create a default-constructed Rating
Rating CreateDefaultRating() noexcept;

///Create a test (and completely bogus) Rating
Rating CreateTestRating() noexcept;

///Exract a Rating from an XML string
Rating XmlToRating(const std::string& s);

///Convert a Rating to an XML string
std::string ToXml(const Rating& r);

bool operator==(const Rating& lhs, const Rating& rhs) noexcept;
bool operator!=(const Rating& lhs, const Rating& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const Rating& r) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPRATING_H
