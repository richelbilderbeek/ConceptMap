#ifndef CONCEPTMAPCONCEPTMAP_H
#define CONCEPTMAPCONCEPTMAP_H

#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "install_vertex_custom_type.h"
#include "install_vertex_is_selected.h"
#include "install_edge_custom_type.h"
#include "install_edge_is_selected.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "conceptmapgraphtypes.h"
namespace ribi {
namespace cmap {

//Concept map definition is in 'conceptmapgraphtypes.h'

///Calculate the complexity of a concept map from human input
int CalculateComplexityExperimental(const ConceptMap& c);

///Calculate the complexity of a concept map from the graph only,
///that is, without the interference of humans
int CalculateComplexityEstimated(const ConceptMap& c);

///Calculate the richness of a concept map,
///which is determined by a tally of competencies that
///are scored by a human
int CalculateRichnessExperimental(const ConceptMap& c);

///Calculate the richness of a concept map, from a tally of competencies
int CalculateRichnessExperimental(std::map<cmap::Competency,int> m);

///Collect all competenties of a concept map, with no intended
///ordering. Used to tally the competencies by TallyCompetencies
std::vector<Competency> CollectCompetenies(const ConceptMap& c) noexcept;

int CountCenterNodes(const ConceptMap& c) noexcept;
int CountSelectedEdges(const ConceptMap& c) noexcept;
int CountSelectedNodes(const ConceptMap& c) noexcept;

ConceptMap CreateDirectNeighbourConceptMap(const VertexDescriptor vd, const ConceptMap& c);
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
std::pair<Node, Node> GetFromTo(const EdgeDescriptor ed, const ConceptMap& c);

Node GetNode(const VertexDescriptor vd, const ConceptMap& c) noexcept;

Node GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetTo(const Edge& edge, const ConceptMap& c);

/// Because 'get_my_custom_vertexes(c)' does not work yet
std::vector<Node> GetNodes(const ConceptMap& c) noexcept;

std::vector<Edge> GetSortedEdges(const ConceptMap& c) noexcept;
std::vector<Node> GetSortedNodes(const ConceptMap& c) noexcept;
bool HasCenterNode(const ConceptMap& c) noexcept;

///Are the examples present that are not rated yet?
bool HasUninitializedExamples(const ConceptMap& c) noexcept;

///Is the Edge connected to the Node?
bool IsConnectedTo(const Edge& edge, const Node& node, const ConceptMap& c);

ConceptMap LoadFromFile(const std::string& dot_filename);
ConceptMap RemoveFirstNode(ConceptMap g);
void SaveToFile(const ConceptMap& g, const std::string& dot_filename);
void SaveToImage(const ConceptMap& g, const std::string& png_filename);
void SaveSummaryToImage(const ConceptMap& g, const std::string& png_filename);
void SaveSummaryToFile(const ConceptMap& g, const std::string& dot_filename);
void SelectRandomNode(ConceptMap& conceptmap, std::mt19937& rng_engine);

std::map<Competency,int> TallyCompetencies(const ConceptMap& g) noexcept;

std::string ToXml(const ConceptMap& conceptmap) noexcept;
std::string ToDot(const ConceptMap& conceptmap) noexcept;
ConceptMap UnselectEdges(ConceptMap conceptmap) noexcept;
ConceptMap UnselectEverything(ConceptMap conceptmap) noexcept;
ConceptMap UnselectNodes(ConceptMap conceptmap) noexcept;
ConceptMap XmlToConceptMap(const std::string& s);
std::ostream& operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept;
std::istream& operator>>(std::istream& is, ConceptMap& conceptmap);
bool operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
bool operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAP_H
