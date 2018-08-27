#ifndef CONCEPTMAPHELPER_H
#define CONCEPTMAPHELPER_H

#include <array>
#include <map>
#include <memory>
#include <vector>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

#include <boost/array.hpp>
#include <boost/numeric/conversion/cast.hpp>


namespace ribi {

namespace cmap {

template <class T>
int CountTallyTotal(const std::map<T, int>& t)
{
  return std::accumulate(std::begin(t), std::end(t),0,
    [](int& init,const auto& p)
    {
      return init + p.second;
    }
  );
}

///Create a tally of value occurrances
///For example {"A", "A", "B"} would result in {{"A",2},{"B",1}}
///The sum of the tally frequencies will be equal to the length of the input vector
template <class T>
std::map<T, int> CreateTally(
  const std::vector<T>& v
) noexcept
{
  std::map<T, int> m;
  for (const auto& i: v)
  {
    const auto iter = m.find(i);
    if (iter == std::end(m))
    {
      m.insert(std::make_pair(i, 1));
    }
    else { ++m[i]; }
  }
  return m;
}

///Find the last space before the ith position
std::size_t FindLastSpaceBeforeMaxLen(
  const std::string& s,
  const std::size_t max_len
);

///Detects if this code is run on a Travis CI server
bool OnTravis() noexcept;

std::vector<std::string> ReplaceAll(
  std::vector<std::string> v,
  const char from,
  const char to
);

///Starting from the start of the std::string,
///as long as there is a space, replace it by the character desired
std::string ReplaceLeadingSpaces(std::string s, const char x);

///Does ReplaceLeadingSpaces and ReplaceTrailingSpaces
std::string ReplaceLeadingAndTrailingSpaces(std::string s, const char x);

///Starting from the end of the std::string,
///as long as there is a space, replace it by the character desired
std::string ReplaceTrailingSpaces(std::string s, const char x);

///SafeFileToVector calls FileToVector and
///removes an empty trailing line that can be created under
///the Windows operating system, due to different line endings
std::vector<std::string> SafeFileToVector(const std::string& filename);

///Undo a Wordwrap
std::string Unwordwrap(const std::vector<std::string>& v) noexcept;

///Wordwrap the text to obtain lines of max_len characters
///If the string _must_ be seperable by spaces; a word can have a maximum length of max_len
std::vector<std::string> Wordwrap(std::string s, const std::size_t max_len);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPHELPER_H
