#ifndef CONCEPTMAPCONCEPTMAPFACTORY_H
#define CONCEPTMAPCONCEPTMAPFACTORY_H

#include <vector>
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

struct Node;

class ConceptMapFactory
{
public:
  using ConceptMaps = std::vector<ConceptMap>;

  ConceptMapFactory() noexcept;

  ///Get all the other tests as one vector
  ConceptMaps GetAllTests() const noexcept;
  ConceptMaps GetNastyTests() const noexcept;
  ConceptMap GetEmptyGraph() const noexcept { return Get0(); }

  ///Get the documented heteromorphous test concept maps
  ConceptMap Get0() const noexcept; //Empty graph
  ConceptMap Get1() const noexcept;
  ConceptMap Get2() const noexcept;
  ConceptMap Get3() const noexcept;
  ConceptMap Get4() const noexcept;
  ConceptMap Get5() const noexcept;
  ConceptMap Get6() const noexcept;
  ConceptMap Get7() const noexcept;

  /// Complex rated concept map
  ConceptMap Get8() const noexcept;

  /// Complex rated concept map with many examples
  ConceptMap Get9() const noexcept;

  /// Complex rated concept map with many long concept names and examples
  ConceptMap Get10() const noexcept;

  /// Complex rated concept map with all nodes connected
  ConceptMap Get11() const noexcept;

  ///Create a concept map in which the concept's name is its level
  /// 3 <- 2 <- 1 <- 0 -> 1 -> 2
  ///      |         |    |
  ///      v         v    v
  ///      3         1    2
  ConceptMap GetLevel() const noexcept;

  /// Just one center node with text 'center'
  ConceptMap GetLonelyQtCenterNode() const noexcept;

  /// Concept map with one node connected to the focal question
  /// All other nodes are connected to that non-focal node
  ConceptMap GetStarShaped() const noexcept;

  ///Creates a simple concept map with a center and normal node
  ///The normal node will have examples that are attributed the
  ///competencies supplied.
  ConceptMap GetWithExamplesWithCompetencies(
    const std::vector<Competency>& competencies
  ) const noexcept;

  ConceptMap GetNasty0() const noexcept;

  ///Example Used by QtRateConceptTallyDialog
  ConceptMap GetRateConceptTallyDialogExample() const noexcept;

  ///Example Used by QtRateConceptTallyDialog to resolve #283
  ConceptMap GetRateConceptTallyDialogExample283() const noexcept;

  ///Example Used by QtRatedConceptDialog
  ConceptMap GetQtRatedConceptDialogExample() const noexcept;

  ///Rated and human-friendly concept map
  ConceptMap GetRated() const noexcept;

  /// center --- one --- two
  ///       first second
  ConceptMap GetThreeNodeTwoEdge() const noexcept;

  /// one --- two --- three
  ///    first  second
  ConceptMap GetThreeNodeTwoEdgeNoCenter() const noexcept;

  /// center ----> one
  ///        first
  ConceptMap GetTwoNodeOneEdge() const noexcept;

  /// one --- two
  ///    second
  ConceptMap GetTwoNodeOneEdgeNoCenter() const noexcept;

  ///Unrated and human-friendly concept map
  ConceptMap GetUnrated() const noexcept;

};

void AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  ConceptMap& g
) noexcept;

VertexDescriptor AddVertex(const Node& node, ConceptMap& g) noexcept;

///Reposition the nodes and edges to nice default locations
ConceptMap Reposition(ConceptMap& g);


} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
