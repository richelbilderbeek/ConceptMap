#ifndef CONCEPTMAPEXAMPLES_H
#define CONCEPTMAPEXAMPLES_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <vector>
#include "conceptmapfwd.h"
#include "conceptmapexample.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Container of Example instances
///Examples is displayed by
/// - ?QtExamplesItem: a QGraphicsItem, to be used in a QGraphicsView
/// - QtExamplesDialog: a QDialog
struct Examples
{
  explicit Examples(const std::vector<Example>& v = {});

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept;

  const std::vector<Example>& Get() const noexcept { return m_v; }
        std::vector<Example>& Get()       noexcept { return m_v; }

  std::string ToStr() const noexcept;

private:

  std::vector<Example> m_v;
};

bool IsEmpty(const Examples& examples) noexcept;

std::string ToXml(const Examples& node) noexcept;
Examples XmlToExamples(const std::string& s);

std::ostream& operator<<(std::ostream& os, const Examples& concept) noexcept;
std::istream& operator>>(std::istream& is, Examples& concept);

bool operator==(const Examples& lhs, const Examples& rhs);
bool operator!=(const Examples& lhs, const Examples& rhs);

///Two cmap::Examples instances are sorted as follows:
///(1) By their number of examples
///(2) (if the sizes are equal) Alphabetically on the first different example
bool operator<(const Examples& lhs, const Examples& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEXAMPLES_H
