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

  // { {number of edges, number of examples}, score }
  RatingComplexity(
    const std::map<std::pair<int, int>, int>& rating_complexity
  );

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const int n_edges,
    const int n_examples
  ) const;

  ///Calculate a suggested complexity in the default way
  static int SuggestComplexityDefault(const int n_edges, const int n_examples); //!OCLINT static because it needs no member variables

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const;

  private:
  // { {number of edges, number of examples}, score }
  std::map<std::pair<int, int>, int> m_rating;

  friend bool operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
  friend std::string ToXml(const RatingComplexity& rating);
};

RatingComplexity CreateDefaultRatingComplexity() noexcept;

RatingComplexity CreateTestRatingComplexity() noexcept;

///Exract a RatingComplexity from an XML string
RatingComplexity XmlToRatingComplexity(const std::string& s);

///To be used as a tooltip
///@param rating a Rating
///@param n_examples_emph the number of complex examples that will
///   be emphasized. No column will be highlighted
///   as such for invalid values
///@param n_relations_emph the number of relations that will
///   be emphasized. No row will be highlighted
///   as such for invalid values
std::string ToHtml(
  const RatingComplexity& rating,
  const int n_examples_emph = -1,
  const int n_relations_emph = 1
);

///Only create the header of ToHtml for rating compleXity
std::string ToHtmlHeaderX(
  const int n_examples_emph
);

std::string ToXml(const RatingComplexity& rating);

bool operator==(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
bool operator!=(const RatingComplexity& lhs, const RatingComplexity& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const RatingComplexity& r) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPRATINGCOMPLEXITY_H
