#ifndef CONCEPTMAPEDGE_H
#define CONCEPTMAPEDGE_H

#include "conceptmapfwd.h"
#include "conceptmapnode.h"


namespace ribi {
namespace cmap {

struct EdgeFactory;

///An Edge is the GUI-independent part of the edges used in QtConceptMap.
///An Edge goes from one Node to another, which must a different Node,
/// at the center of the Edge is a Node
struct Edge
{
  ///@param node a Node
  ///@param has_head_arrow does the edge have an arrow at its head/target
  ///@param has_tail_arrow does the edge have an arrow at its tail/source
  explicit Edge(
    const Node& node = Node(),
    const bool has_head_arrow = false,
    const bool has_tail_arrow = false
  );

  ~Edge() noexcept;

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept { m_node.Decode(); }

  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }

  bool HasHeadArrow() const noexcept { return m_has_head_arrow; }
  bool HasTailArrow() const noexcept { return m_has_tail_arrow; }

  int GetId() const noexcept { return m_id; }

  ///These arrows are needed: the graph is an undirected graph
  ///with/without arrows at either/both sides
  ///This cannot be done by using a directed
  void SetHeadArrow(const bool has_head_arrow) noexcept { m_has_head_arrow = has_head_arrow; }
  void SetTailArrow(const bool has_tail_arrow) noexcept { m_has_tail_arrow = has_tail_arrow; }

  ///Set the center Node
  void SetNode(const Node& node) noexcept;

  std::string ToStr() const noexcept;


  private:
  bool m_has_head_arrow;
  bool m_has_tail_arrow;

  int m_id; //Unique ID, until the Node is copied

  ///The Node on the Edge
  Node m_node;

  //ID to assign
  static int sm_ids; //!OCLINT Easiest to track instances by static
};

///Collect the texts of all examples
std::vector<std::string> CollectExamplesTexts(const Edge& edge) noexcept;

/// Count the number of examples on the edge's concept
int CountExamples(const Edge& edge) noexcept;

const Concept& GetConcept(const Edge& edge) noexcept;

const Examples& GetExamples(const Edge& edge) noexcept;

///The concept on an Edge (only) being complex,
///as rated by an assessor.
///This is different than an example being complex.
bool GetIsComplex(const Edge& edge) noexcept;

std::string GetText(const Edge& edge) noexcept;

///Get the x coordinat of the center of the Edge
double GetX(const Edge& edge) noexcept;

///Get the x coordinat of the center of the Edge
double GetY(const Edge& edge) noexcept;

///Does the (Node on the) Edge have at least one example?
bool HasExamples(const Edge& e) noexcept;

///Checks if the nodes and arrows are the same
bool HasSameData(const Edge& lhs, const Edge& rhs) noexcept;

///Pairwise checks if all Edges at left- and right-hand-side have the same data
///Throws if the sizes differ
bool HasSameData(const std::vector<Edge>& lhs, const std::vector<Edge>& rhs);

///Checks if the nodes and arrows are similar
bool HasSimilarData(const Edge& lhs, const Edge& rhs, const double tolerance) noexcept;

///Pairwise checks if all Edges at left- and right-hand-side have similar data (there
///is some tolerance in the coordinats)
///Throws if the sizes differ
bool HasSimilarData(
  const std::vector<Edge>& lhs,
  const std::vector<Edge>& rhs,
  const double tolerance
);

///Move an Edge relatively
void Move(Edge& edge, const double dx, const double dy);

void SetConcept(Edge& edge, const Concept& concept) noexcept;

std::string ToXml(const Edge& edge) noexcept;
Edge XmlToEdge(const std::string& s);
std::ostream& operator<<(std::ostream& os, const Edge& edge) noexcept;
std::istream& operator>>(std::istream& is, Edge& edge);

bool operator==(const Edge& lhs, const Edge& rhs);
bool operator!=(const Edge& lhs, const Edge& rhs);
bool operator<(const Edge& lhs, const Edge& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGE_H
