#ifndef CONCEPTMAPNODE_H
#define CONCEPTMAPNODE_H





#include "conceptmapfwd.h"
#include "conceptmapconcept.h"
#include "conceptmapnodetype.h"


namespace ribi {
namespace cmap {

struct NodeFactory;

///A Node is a Concept with coordinats
///A Node is the GUI independent part of a node. It is displayed as:
struct Node
{
  /// @param id the ID. If -1, it will be assigned to a positive number.
  ///   Else, the id will be passed
  ///
  explicit Node(
    const Concept& concept = Concept(),
    const NodeType node_type = NodeType::normal,
    const double x = 0.0,
    const double y = 0.0,
    const int id = -1
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
  auto GetX() const noexcept { return m_x; }

  ///Get the y coordinat
  auto GetY() const noexcept { return m_y; }

  NodeType GetType() const noexcept { return m_type; }

  ///Set the concept
  void SetConcept(const Concept& concept) noexcept;

  void SetType(const NodeType type) noexcept { m_type = type; }

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

  NodeType m_type;

  ///The x-coordinat
  double m_x;

  ///The y-coordinat
  double m_y;

  static int sm_ids; //!OCLINT ID to assign, static is easiest to track number of instances

  ///If id is positive, copy it
  ///If id is negative, create a new ID and return it
  static int AssignId(const int id); //!OCLINT

  //friend bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;
};

///Collect the texts of all examples
std::vector<std::string> CollectExamplesTexts(const Node& node) noexcept;

///Count the number of CenterNodes
int CountCenterNodes(const std::vector<Node>& nodes) noexcept;

/// Count the number of examples
int CountExamples(const Node& node) noexcept;

NodeType ExtractIsCenterNodeFromXml(const std::string& s);
double ExtractXfromXml(const std::string& s);
double ExtractYfromXml(const std::string& s);

const Concept& GetConcept(const Node& node) noexcept;

const Examples& GetExamples(const Node& node) noexcept;

///The concept on an Edge (only) being complex,
///as rated by an assessor.
///This is different than an example being complex.
bool GetIsComplex(const Node& node) noexcept;

int GetRatingComplexity(const Node& node) noexcept;
int GetRatingConcreteness(const Node& node) noexcept;
int GetRatingSpecificity(const Node& node) noexcept;

std::string GetText(const Node& node) noexcept;

///Get the x coordinat of the center of the Node
inline auto GetX(const Node& node) noexcept { return node.GetX(); }

///Get the x coordinat of the center of the Node
inline auto GetY(const Node& node) noexcept { return node.GetY(); }

bool HasExamples(const Node& node) noexcept;

///Checks if the Concepts are the same
bool HasSameContent(const Node& lhs, const Node& rhs) noexcept;

///Checks if the concepts and coordinats are the same
bool HasSameData(const Node& lhs, const Node& rhs) noexcept;

///Pairwise checks if all Nodes at left- and right-hand-side have the same data
///Throws if the sizes differ
bool HasSameData(const std::vector<Node>& lhs, const std::vector<Node>& rhs);

///Checks if the concepts and coordinats are similar (there is some
///tolerance in the coordinats)
bool HasSimilarData(const Node& lhs, const Node& rhs, const double tolerance) noexcept;

///Pairwise checks if all Nodes at left- and right-hand-side have similar data
///Throws if the sizes differ
bool HasSimilarData(
  const std::vector<Node>& lhs,
  const std::vector<Node>& rhs,
  const double tolerance);

///Returns true if Node is of derived class type CenterNode
///Returns true if Node is Node
bool IsCenterNode(const Node& node) noexcept;

///To uniquely identify all Nodes
bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;

///Move a Node relatively
void Move(Node& node, const double dx, const double dy);

bool NodeHasExamples(const Node& node) noexcept;

///Set the Concept of the Node
void SetConcept(Node& node, const Concept& concept);

void SetExamples(Node& node, const Examples& examples) noexcept;

///Set the rating of this Node for complexity
///-1: not rated, 0: lowest, 2: highest
void SetRatingComplexity(Node& node, const int rating_complexity);

///Set the rating of this Node for concreteness
///-1: not rated, 0: lowest, 2: highest
void SetRatingConcreteness(Node& node, const int rating_concreteness);

///Set the rating of this Node for specificity
///-1: not rated, 0: lowest, 2: highest
void SetRatingSpecificity(Node& node, const int rating_specificity);

///Set the name/text of the Node
void SetText(Node& node, const std::string& text);

///Set the x coordinat of the center of the Node
inline void SetX(Node& node, const double x) noexcept { node.SetX(x); }

///Set the x coordinat of the center of the Node
inline void SetY(Node& node, const double y) noexcept { node.SetY(y); }

std::string ToXml(const Node& node) noexcept;
Node XmlToNode(const std::string& s);

///Checks if Nodes are identical
bool operator==(const Node& lhs, const Node& rhs) noexcept;

bool operator!=(const Node& lhs, const Node& rhs) noexcept;
bool operator<(const Node& lhs, const Node& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Node& node) noexcept;
std::istream& operator>>(std::istream& is, Node& node);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPNODE_H
