//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPHELPER_H
#define CONCEPTMAPHELPER_H

#include <array>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/array.hpp>
#include <boost/numeric/conversion/cast.hpp>
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

///Obtain all possible selections of a std::vector, preserving the ordering of its elements
///Examples:
/// {     } -> { {}                                              }
/// {1    } -> { {}, {1}                                         }
/// {1,2  } -> { {}, {1}, {2},      {1,2}                        }
/// {1,2,3} -> { {}, {1}, {2}, {3}, {1,2}, {1,3}, {2,3}, {1,2,3} }
//From http://www.richelbilderbeek.nl/GetPermutations.htm
template <class T>
const std::vector<std::vector<T> > GetCombinations(const std::vector<T>& v)
{
  std::vector<std::vector<T> > result;
  const int sz = boost::numeric_cast<int>(v.size());
  const int n_combinations{1 << sz};

  for (int i=0; i!=n_combinations; ++i)
  {
    std::vector<T> w;
    for (int j=0; j!=sz; ++j)
    {
      const int is_exponent{(1 << j) & i};
      if (is_exponent)
      {
        w.push_back(v[j]);
      }
    }
    result.push_back(w);
  }
  return result;
}

///Find the last space before the ith position
std::size_t FindLastSpaceBeforeMaxLen(
  const std::string& s,
  const std::size_t max_len
);

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
std::vector<std::string> Wordwrap(std::string s, const std::size_t max_len) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPHELPER_H
