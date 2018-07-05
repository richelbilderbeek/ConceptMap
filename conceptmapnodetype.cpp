#include "conceptmapnodetype.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

ribi::cmap::NodeType ribi::cmap::ToNodeType(const std::string& s)
{
  if (s == "center") return NodeType::center;
  if (s == "normal") return NodeType::normal;
  throw std::invalid_argument("string cannot be converted to NodeType");
}

std::string ribi::cmap::ToStr(const NodeType t) noexcept
{
  if (t == NodeType::center)
  {
    return "center";
  }
  assert(t == NodeType::normal);
  return "normal";
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const NodeType t) noexcept
{
  os << ToStr(t);
  return os;
}
