#ifndef CONCEPTMAPNODE_H
#define CONCEPTMAPNODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapfwd.h"
#include "conceptmapconcept.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct NodeFactory;

///A Node is a Concept with coordinats
///A Node is the GUI independent part of a node. It is displayed as:
struct Node
{
  explicit Node(
    const Concept& concept = Concept(),
    const bool is_center_node = false,
    const double x = 0.0,
    const double y = 0.0
  ) noexcept;

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept { m_concept.Decode(); }

  ///Get the Concept
  const Concept& GetConcept() const noexcept { return m_concept; }
        Concept& GetConcept()       noexcept { return m_concept; }

  ///Get some test nodes
  static std::vector<Node> GetTests() noexcept;

  int GetId() const noexcept { return m_id; }

  ///Get the text on the concept
  const std::string& GetName() const noexcept { return m_concept.GetName(); }

  ///Get the x coordinat
  double GetX() const noexcept { return m_x; }

  ///Get the y coordinat
  double GetY() const noexcept { return m_y; }

  bool IsCenterNode() const noexcept { return m_is_center_node; }

  ///Set the concept
  void SetConcept(const Concept& concept) noexcept;

  void SetIsCenterNode(const bool is_center_node) noexcept { m_is_center_node = is_center_node; }

  ///Set the position
  void SetPos(const double x, const double y) noexcept { SetX(x); SetY(y); }

  ///Set the x coordinat
  void SetX(const double x) noexcept;

  ///Set the y coordinat
  void SetY(const double y) noexcept;

  std::string ToStr() const noexcept;

  protected:
  friend class NodeFactory;
  friend class CenterNodeFactory;


  private:

  ///The Concept
  Concept m_concept;

  int m_id; // Unique ID

  bool m_is_center_node;

  ///The x-coordinat
  double m_x;

  ///The y-coordinat
  double m_y;

  static int sm_ids; //!OCLINT ID to assign, static is easiest to track number of instances

  //friend bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;
};

///Count the number of CenterNodes
int CountCenterNodes(const std::vector<Node>& nodes) noexcept;


bool ExtractIsCenterNodeFromXml(const std::string& s);
double ExtractXfromXml(const std::string& s);
double ExtractYfromXml(const std::string& s);

//std::vector<Node>::const_iterator FindCenterNode(const std::vector<Node>& nodes) noexcept;

std::string GetText(const Node& node) noexcept;

bool HasExamples(const Node& node) noexcept;
bool NodeHasExamples(const Node& node) noexcept;

///Similar to operator==, except that GUI elements are not tested for equality
bool HasSameContent(const Node& lhs, const Node& rhs) noexcept;

///Returns true if Node is of derived class type CenterNode
///Returns true if Node is Node
bool IsCenterNode(const Node& node) noexcept;

///To uniquely identify all Nodes
bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;

std::string ToXml(const Node& node) noexcept;
Node XmlToNode(const std::string& s);

bool operator==(const Node& lhs, const Node& rhs) noexcept;
bool operator!=(const Node& lhs, const Node& rhs) noexcept;
bool operator<(const Node& lhs, const Node& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Node& node) noexcept;
std::istream& operator>>(std::istream& is, Node& node);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPNODE_H
