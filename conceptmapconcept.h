#ifndef CONCEPTMAPCONCEPT_H
#define CONCEPTMAPCONCEPT_H

#include <string>
#include <vector>

#include "conceptmapexamples.h"

namespace ribi {
namespace cmap {

struct ConceptFactory;

///A Concept is a class that has a name and examples
///A Concept is a Node without coordinats
///A Concept is an Edge without coordinats, source and target
struct Concept
{
  // A relation is complex by default, as requested by employer:
  // https://github.com/richelbilderbeek/BrainWeaver/issues/221
  explicit Concept(
    const std::string& name = "...",
    const Examples& examples = Examples(),
    const bool is_complex = true,
    const int rating_complexity = -1,
    const int rating_concreteness = -1,
    const int rating_specificity = -1
  );

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept;

  ///Get the examples of the concept, e.g. 'Plato', 'Aristotle'
  const Examples& GetExamples() const noexcept { return m_examples; }
        Examples& GetExamples()       noexcept { return m_examples; }

  ///The concept on an Edge (only) being complex,
  ///as rated by an assessor.
  ///This is different than an example being complex.
  bool GetIsComplex() const noexcept { return m_is_complex; }

  ///Get the name of the concept, e.g. 'Philosphy'
  const std::string& GetName() const noexcept { return m_name; }

  ///Get the rating of this Concept for complexity
  ///-1: not rated, 0: lowest, 2: highest
  int GetRatingComplexity() const noexcept { return m_rating_complexity; }

  ///Get the rating of this Concept for concreteness
  ///-1: not rated, 0: lowest, 2: highest
  int GetRatingConcreteness() const noexcept { return m_rating_concreteness; }

  ///Get the rating of this Concept for specificity
  ///-1: not rated, 0: lowest, 2: highest
  int GetRatingSpecificity() const noexcept { return m_rating_specificity; }

  ///Set the examples
  void SetExamples(const Examples& examples) noexcept;

  ///Has an assessor rated the name of this concept as being an addition to the complexity?
  ///This is something different than m_rating_complexity:
  ///m_is_complex can be used to help the assessor determine a m_rating_complexity,
  ///but m_rating_complexity is the final and complete rating
  void SetIsComplex(const bool is_complex) noexcept;

  ///Set the name
  void SetName(const std::string& name) noexcept;

  ///Set the rating of this Concept for complexity
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingComplexity(const int rating_complexity);

  ///Set the rating of this Concept for concreteness
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingConcreteness(const int rating_concreteness);

  ///Set the rating of this Concept for specificity
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingSpecificity(const int rating_specificity);

  ///Convert Concept to a short std::string
  std::string ToStr() const noexcept;

  private:

  ///Examples of the concept, e.g. 'Plato', 'Aristotle'
  Examples m_examples;

  ///Has an assessor rated the name of this concept as being an addition to the complexity?
  ///This is something different than m_rating_complexity:
  ///m_is_complex can be used to help the assessor determine a m_rating_complexity,
  ///but m_rating_complexity is the final and complete rating
  bool m_is_complex;

  ///The name of the concept, e.g. 'Philosphy'
  std::string m_name;

  ///The rating of this Concept for complexity
  ///-1: not rated, 0: lowest, 2: highest
  int m_rating_complexity;

  ///The rating of this Concept for concreteness
  ///-1: not rated, 0: lowest, 2: highest
  int m_rating_concreteness;

  ///The rating of this Concept for specificity
  ///-1: not rated, 0: lowest, 2: highest
  int m_rating_specificity;
};

///Collect the texts of all examples
std::vector<std::string> CollectExamplesTexts(const Concept& concept) noexcept;

///Count the number of examples
int CountExamples(const Concept& concept) noexcept;

///Loose version of XmlToConcept
Concept ExtractConceptFromXml(const std::string& s);

///Extract a Concept its Examples from an XML
Examples ExtractExamplesFromXml(const std::string& s);

///Extract a Concept if it is complex from an XML
bool ExtractIsComplexFromXml(const std::string& s);

///Extract a Concept its name from an XML
std::string ExtractNameFromXml(const std::string& s);

///Extract a Concept its complexity rating from XML
int ExtractRatingComplexityFromXml(const std::string& s);

///Extract a Concept its concreteness rating from XML
int ExtractRatingConcretenessFromXml(const std::string& s);

///Extract a Concept its specificity rating from XML
int ExtractRatingSpecificityFromXml(const std::string& s);

///Get the example at index i.
///Assumes i is a valid index
const Example& GetExample(const Concept& concept, const int i);
      Example& GetExample(      Concept& concept, const int i);

const Examples& GetExamples(const Concept& concept) noexcept;
      Examples& GetExamples(      Concept& concept) noexcept;


///The concept on an Edge (only) being complex,
///as rated by an assessor.
///This is different than an example being complex.
bool GetIsComplex(const Concept& concept) noexcept;

int GetRatingComplexity(const Concept& concept) noexcept;
int GetRatingConcreteness(const Concept& concept) noexcept;
int GetRatingSpecificity(const Concept& concept) noexcept;

const std::string& GetName(const Concept& concept) noexcept;
const std::string& GetText(const Concept& concept) noexcept;

///Does the Concept have at least one example?
bool HasExamples(const Concept& concept) noexcept;

bool IsComplex(const Concept& concept) noexcept;

void SetExamples(Concept& concept, const Examples& examples) noexcept;

///Set the tallied complexity
void SetIsComplex(Concept& concept, const bool is_complex = true);

///Set the rating of this Concept for complexity
///-1: not rated, 0: lowest, 2: highest
void SetRatingComplexity(Concept& concept, const int rating_complexity);

///Set the rating of this Concept for concreteness
///-1: not rated, 0: lowest, 2: highest
void SetRatingConcreteness(Concept& concept, const int rating_concreteness);

///Set the rating of this Concept for specificity
///-1: not rated, 0: lowest, 2: highest
void SetRatingSpecificity(Concept& concept, const int rating_specificity);

///Set the name/text of a concept
void SetText(Concept& concept, const std::string& text);

std::string ToXml(const Concept& concept) noexcept;

///Strict version of ExtractConceptFromXml
Concept XmlToConcept(const std::string& s);

std::ostream& operator<<(std::ostream& os, const Concept& concept) noexcept;
std::istream& operator>>(std::istream& is, Concept& concept);

bool operator==(const Concept& lhs, const Concept& rhs);
bool operator!=(const Concept& lhs, const Concept& rhs);

///Two Concept instances are ordered as follows:
///(1) Alphabetically on the name
///(2) (if the names are equal) On their Examples
bool operator<(const Concept& lhs, const Concept& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPT_H
