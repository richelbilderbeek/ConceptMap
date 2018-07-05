#ifndef CONCEPTMAPNODETYPE_H
#define CONCEPTMAPNODETYPE_H

#include <iosfwd>
#include <string>

namespace ribi {
namespace cmap {

enum class NodeType
{
  normal,
  center
};

std::string ToStr(const NodeType t) noexcept;

NodeType ToNodeType(const std::string& s);

std::ostream& operator<<(std::ostream& os, const NodeType t) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPNODETYPE_H
