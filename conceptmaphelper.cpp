



#include "conceptmaphelper.h"

#include <cassert>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <QFile>

#include "fileio.h"
#include "conceptmapregex.h"

std::size_t ribi::cmap::FindLastSpaceBeforeMaxLen(
  const std::string& s,
  const std::size_t max_len
)
{
  std::size_t len = s.find(' ');
  assert(len != std::string::npos);
  assert(len < s.size());
  while (1)
  {
    const std::size_t new_len = s.find(' ', len + 1);
    if (new_len > max_len || new_len == std::string::npos) break;
    len = new_len;
  }
  return len;
}


bool ribi::cmap::OnTravis() noexcept
{
  return std::getenv("TRAVIS");
}

std::vector<std::string> ribi::cmap::ReplaceAll(
  std::vector<std::string> v,
  const char from,
  const char to
)
{
  ///Replace bell characters by spaces again
  for (std::string& s: v)
  {
    std::replace(std::begin(s), std::end(s), from, to);
  }
  return v;
}

std::string ribi::cmap::ReplaceLeadingSpaces(std::string s, const char x)
{
  const std::size_t sz = s.size();

  for (std::size_t i=0; i!=sz; ++i)
  {
    if (s[i] == ' ')
      s[i] = x;
    else
      break;
  }
  return s;
}

std::string ribi::cmap::ReplaceLeadingAndTrailingSpaces(
  std::string s, const char x)
{
  return ReplaceTrailingSpaces(
    ReplaceLeadingSpaces(s, x),
    x
  );
}

std::string ribi::cmap::ReplaceTrailingSpaces(std::string s, const char x)
{
  const int sz = static_cast<int>(s.size());
  if (sz == 0) return s;

  //i!=0, because if s[0] is a space, it is already converted to bell
  for (int i = sz - 1; i != 0; --i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(s.size()));
    if (s[i] == ' ')
      s[i] = x;
    else
      break;
  }
  return s;
}

std::vector<std::string> ribi::cmap::SafeFileToVector(const std::string& filename)
{
  if(!ribi::is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  std::vector<std::string> v = ribi::FileIo().FileToVector(filename);
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}

std::string ribi::cmap::Unwordwrap(const std::vector<std::string>& v) noexcept
{
  //Simply concatenate
  std::string t;
  for (const std::string& s: v) { t += s; }
  return t;
}

std::vector<std::string> ribi::cmap::Wordwrap(
  std::string s, const std::size_t max_len)
{
  if (max_len == 0)
  {
    throw std::invalid_argument("Cannot wordwrap for a max_len of zero");
  }

  ///Replace multiple spaces with '\b ', where x is a char not in the string
  std::string::value_type x = '\b'; //Bell

  s = ReplaceLeadingAndTrailingSpaces(s, x);

  std::vector<std::string> v;

  //Start the actual wordwrapping
  while (1)
  {
    //Is the word short enough to be the last entry in v?
    if (s.size() < max_len)
    {
      //Copy last part to v and it is done
      v.push_back(s);
      break;
    }
    //No spaces, or space beyond max_len: cut word
    if (s.find(' ') == std::string::npos || s.find(' ') > max_len)
    {
      v.push_back(s.substr(0,max_len));
      s = s.substr(max_len,s.size() - max_len);
      continue;
    }
    //Find last space before max_len
    const std::size_t len = FindLastSpaceBeforeMaxLen(s, max_len);
    assert(len + 1 < s.size());
    //cut s, put cut part in vector
    const std::string line = s.substr(0,len+1); //Keep space
    assert(!line.empty());
    v.push_back(line);
    const std::size_t new_index = len+1; //After the space found
    assert(new_index < s.size());
    const std::string new_s = s.substr(new_index,s.size() - new_index);
    assert(s != new_s);
    s = new_s;
  }

  return ReplaceAll(v, x, ' ');
}
