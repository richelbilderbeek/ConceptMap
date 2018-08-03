#ifndef CONCEPTMAPCONCEPTMAP_H
#define CONCEPTMAPCONCEPTMAP_H

#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

//Concept map definition is in 'conceptmapgraphtypes.h'

///Calculate the complexity of a concept map from human input
int CalculateComplexityExperimental(const ConceptMap& c);

///Calculate the concreteness of a concept map from human input
int CalculateConcretenessExperimental(const ConceptMap& c);

///Calculate the richness of a concept map,
///which is determined by a tally of competencies that
///are scored by a human
int CalculateRichnessExperimental(const ConceptMap& c);

///Calculate the richness of a concept map, from a tally of competencies
int CalculateRichnessExperimental(std::map<cmap::Competency,int> m);

///Calculate the specificity of a concept map from human input
int CalculateSpecificityExperimental(const ConceptMap& c);

///Collect all competenties of a concept map, with no intended
///ordering. Used to tally the competencies by TallyCompetencies
std::vector<Competency> CollectCompetenies(const ConceptMap& c) noexcept;

///Count the number of center nodes.
///BrainWeaver students work on a concept map with exactly one center node.
///BrainWeaver assessors grade a sub-conceptmap
/// (created with CreateDirectNeighbourConceptMap),
/// that may have the center node excluded.
int CountCenterNodes(const ConceptMap& c) noexcept;

///Count the total number of examples, on both Edges and Nodes
int CountExamples(const ConceptMap& c) noexcept;

///Create a concept map of only the vertex and its direct neighbours itself.
///Use CreateDirectNeighbourConceptMaps to create all sub-conceptmaps.
ConceptMap CreateDirectNeighbourConceptMap(const VertexDescriptor vd, const ConceptMap& c);


///Create all sub-conceptmaps
///Use CreateDirectNeighbourConceptMap to create a sub-conceptmaps
/// with any vertex as its center.
std::vector<ConceptMap> CreateDirectNeighbourConceptMaps(const ConceptMap& c);

void DecodeConceptMap(ConceptMap& g) noexcept;
void DecodeEdge(Edge& g) noexcept;
void DecodeNode(Node& g) noexcept;
ConceptMap DotToConceptMap(const std::string& s);
VertexDescriptor FindCenterNode(const ConceptMap& c);
Node GetCenterNode(const ConceptMap& c);
Edge GetEdge(const EdgeDescriptor vd, const ConceptMap& c) noexcept;
std::vector<Edge> GetEdges(const ConceptMap& c) noexcept;
Edge GetFirstEdge(const ConceptMap& c);
Node GetFirstNode(const ConceptMap& c);

std::string GetFocusName(const ConceptMap& sub_conceptmap);

Node GetFrom(const Edge& edge, const ConceptMap& c);
Node GetFrom(const EdgeDescriptor ed, const ConceptMap& c) noexcept;

///Obtain the source/from and target/to of an edge
///Checks if from != to
std::pair<const Node&, const Node&> GetFromTo(const EdgeDescriptor ed, const ConceptMap& c);

const Node& GetNode(const VertexDescriptor vd, const ConceptMap& c) noexcept;

Node GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetTo(const Edge& edge, const ConceptMap& c);

/// Because 'get_my_bundled_vertexes(c)' does not work yet
std::vector<Node> GetNodes(const ConceptMap& c) noexcept;

std::vector<Edge> GetSortedEdges(const ConceptMap& c) noexcept;
std::vector<Node> GetSortedNodes(const ConceptMap& c) noexcept;
bool HasCenterNode(const ConceptMap& c) noexcept;

///Checks if all Nodes and Edges contain the same data and that
///the topology matches. Or: checks for everything except Node and Edge IDs
bool HasSameData(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;

///Checks if all Nodes and Edges contain similar data (the coordinats have
///some tolerance) and that the topology matches. Or: checks for everything except Node and Edge IDs
bool HasSimilarData(
  const ConceptMap& lhs,
  const ConceptMap& rhs,
  const double tolerance) noexcept;

///Are the examples present that are not rated yet?
bool HasUninitializedExamples(const ConceptMap& c) noexcept;

///Is the Edge connected to the Node?
bool IsConnectedTo(const Edge& edge, const Node& node, const ConceptMap& c);

///Is the vertex connected to the center node?
bool IsPrimaryConcept(const VertexDescriptor vd, const ConceptMap& c);

///Is the vertex unconnected to the center node (or: only to primary and
///secondary vertices)?
bool IsSecondaryConcept(const VertexDescriptor vd, const ConceptMap& c);

ConceptMap LoadFromFile(const std::string& dot_filename);
ConceptMap RemoveFirstNode(ConceptMap g);
void SaveToFile(const ConceptMap& g, const std::string& dot_filename);
void SaveToImage(const ConceptMap& g, const std::string& png_filename);
void SaveSummaryToImage(const ConceptMap& g, const std::string& png_filename);
void SaveSummaryToFile(const ConceptMap& g, const std::string& dot_filename);

std::map<Competency,int> TallyCompetencies(const ConceptMap& g) noexcept;

std::string ToXml(const ConceptMap& conceptmap) noexcept;
std::string ToDot(const ConceptMap& conceptmap) noexcept;
ConceptMap XmlToConceptMap(const std::string& s);
std::ostream& operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept;
std::istream& operator>>(std::istream& is, ConceptMap& conceptmap);
bool operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
bool operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAP_H
