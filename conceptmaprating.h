#ifndef CONCEPTMAPRATING_H
#define CONCEPTMAPRATING_H

#include "conceptmapgraphtypes.h"
#include "conceptmapfwd.h"
#include "conceptmapratingcomplexity.h"

namespace ribi {
namespace cmap {


std::map<int, int> CreateDefaultRatingConcreteness() noexcept;
std::map<int, int> CreateTestRatingConcreteness() noexcept;

std::map<int, int> CreateDefaultRatingSpecificity() noexcept;
std::map<int, int> CreateTestRatingSpecificity() noexcept;

///Functions to help with rating
struct Rating
{
  ///Use CreateDefaultRating for a default-constructed Rating
  Rating(
    const RatingComplexity& rating_complexity,
    const std::map<int, int>& rating_concreteness,
    const std::map<int, int>& rating_specificity
  );

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  private:
  RatingComplexity m_rating_complexity;
  std::map<int, int> m_rating_concreteness;
  std::map<int, int> m_rating_specificity;

  friend bool operator==(const Rating& lhs, const Rating& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const Rating& r) noexcept;
};

///Create a default-constructed Rating
Rating CreateDefaultRating() noexcept;

///Create a test (and completely bogus) Rating
Rating CreateTestRating() noexcept;


///Calculate a suggested concreteness
int SuggestConcreteness(const int n_examples) noexcept;

///Calculate a suggested specificity
int SuggestSpecificity(const int n_examples) noexcept;

bool operator==(const Rating& lhs, const Rating& rhs) noexcept;
bool operator!=(const Rating& lhs, const Rating& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const Rating& r) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPRATING_H
