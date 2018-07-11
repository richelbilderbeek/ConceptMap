#include "conceptmap.h"
#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include "conceptmapconcept.h"
#include "conceptmapedgewriter.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodewriter.h"
#include "conceptmapregex.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "create_all_direct_neighbour_bundled_edges_and_vertices_subgraphs.h"
#include "fileio.h"
#include "find_first_bundled_edge_with_my_edge.h"
#include "find_first_bundled_vertex_with_my_vertex.h"
#include "get_my_bundled_edge.h"
#include "graphviz_decode.h"
#include "load_directed_bundled_edges_and_vertices_graph_from_dot.h"
#include "load_undirected_bundled_edges_and_vertices_graph_from_dot.h"
#include "save_bundled_edges_and_vertices_graph_to_dot.h"
#include "select_random_vertex.h"
#include "xml.h"

int ribi::cmap::CalculateComplexityExperimental(const ConceptMap& c)
{
  if (boost::num_vertices(c) <= 1)
  {
    throw std::invalid_argument(
      "Need at least one non-focal node to calculate experimental complexity"
    );
  }

  //Remove the center node to simplify the calculation
  assert(IsCenterNode(GetFirstNode(c)));
  const auto g = ribi::cmap::RemoveFirstNode(c);
  assert(boost::num_vertices(c));

  std::vector<ribi::cmap::Node> nodes = ribi::cmap::GetNodes(g);

  const int sum_rated_complexity //Constant 'k_i'
    = std::accumulate(nodes.begin(),nodes.end(),0,
    [](int& init, const ribi::cmap::Node& node)
    {
      return init + node.GetConcept().GetRatingComplexity();
    }
  );
  return static_cast<int>(
    std::round(
      50.0 * static_cast<double>(sum_rated_complexity)
       / static_cast<double>(nodes.size())
    )
  );

}

int ribi::cmap::CalculateConcretenessExperimental(const ConceptMap& c)
{
  if (boost::num_vertices(c) <= 1)
  {
    throw std::invalid_argument(
      "Need at least one non-focal node to calculate estimated concreteness"
    );
  }
  //Remove the center node to simplify the calculation
  assert(IsCenterNode(GetFirstNode(c)));
  const auto g = RemoveFirstNode(c);

  //The first node removed
  std::vector<Node> nodes = GetNodes(g);
  assert(!nodes.empty());

  const int src // sum_rated_concreteness
    = std::accumulate(std::begin(nodes),std::end(nodes),0,
    [](int& init, const Node& node)
    {
      return init + node.GetConcept().GetRatingConcreteness();
    }
  );
  return static_cast<int>(
    std::round(
      50.0 * static_cast<double>(src)
      / static_cast<double>(nodes.size())
    )
  );
}

int ribi::cmap::CalculateRichnessExperimental(const ConceptMap& c)
{
  assert(!HasUninitializedExamples(c));
  return CalculateRichnessExperimental(TallyCompetencies(c));
}

int ribi::cmap::CalculateRichnessExperimental(
  std::map<cmap::Competency,int> m
)
{
  if (m.count(Competency::uninitialized))
  {
    throw std::invalid_argument(
      "cannot calculate richness from unitialized examples"
    );
  }
  if(m.count(Competency::n_competencies))
  {
    throw std::invalid_argument(
      "cannot calculate richness from examples with invalid competency 'n_competencies'"
    );
  }

  //Remove category 'misc'
  m.erase(cmap::Competency::misc);

  if (m.empty()) return 0;

  //a: the number of different categories, used in equation at page 617
  const int a{static_cast<int>(m.size())};
  assert(a >= 0 && a <= 6);

  const int n_examples{CountTallyTotal(m)};

  //The min number of a compency being used to count as having contributed to an even distribution
  const int my_min = static_cast<int>(std::ceil( static_cast<double>(n_examples) / 12.0));

  //The max number of a compency being used to count as having contributed to an even distribution
  const int my_max = static_cast<int>(std::floor(static_cast<double>(n_examples) /  4.0));

  //The number of competencies having contributed to an even distribution
  const int b = std::count_if(std::begin(m), std::end(m),
    [my_min,my_max](const auto& p)
    {
      return p.second >= my_min && p.second <= my_max;
    }
  );

  return static_cast<int>(
    std::round(
      100.0 * ( static_cast<double>(a+b) / 12.0)
    )
  );

}

int ribi::cmap::CalculateSpecificityExperimental(const ConceptMap& c)
{
  if (boost::num_vertices(c) <= 1)
  {
    throw std::invalid_argument(
      "Need at least one non-focal node to calculate estimated specificity"
    );
  }

  //The first node removed, as this is the focal question
  assert(IsCenterNode(GetFirstNode(c)));
  const auto g = RemoveFirstNode(c);

  const std::vector<Node> nodes = GetNodes(g);

  assert(!nodes.empty());

  const int srs //sum_rated_specificity
    = std::accumulate(std::begin(nodes), std::end(nodes),0,
    [](int& init, const Node& node)
    {
      return init + node.GetConcept().GetRatingSpecificity();
    }
  );
  return static_cast<int>(
    std::round(
      static_cast<double>(50 * srs)
      / static_cast<double>(nodes.size())
    )
  );
}

std::vector<ribi::cmap::Competency> ribi::cmap::CollectCompetenies(
  const ConceptMap& g
) noexcept
{
  std::vector<Competency> v;
  for (const Node& node: GetNodes(g))
  {
    for (const Example& example: node.GetConcept().GetExamples().Get())
    {
      v.push_back(example.GetCompetency());
    }
  }
  return v;
}

int ribi::cmap::CountCenterNodes(const ConceptMap& c) noexcept
{
  return CountCenterNodes(GetNodes(c));
}

int ribi::cmap::CountExamples(const ConceptMap& c) noexcept
{
  int n_examples = 0;
  for (const auto& n: GetNodes(c))
  {
    n_examples += CountExamples(n);
  }
  for (const auto& e: GetEdges(c))
  {
    n_examples += CountExamples(e);
  }
  return n_examples;
}

std::vector<ribi::cmap::ConceptMap>
ribi::cmap::CreateDirectNeighbourConceptMaps(const ConceptMap& c)
{
  return
  create_all_direct_neighbour_bundled_edges_and_vertices_subgraphs(c);
}

void ribi::cmap::DecodeConceptMap(ConceptMap& g) noexcept
{
  const auto vip = vertices(g);
  std::for_each(vip.first, vip.second,
    [&g](const VertexDescriptor vd)
    {
      Node& node = g[vd];
      node.Decode();
      g[vd] = node;
    }
  );

  const auto eip = edges(g);
  std::for_each(eip.first, eip.second,
    [&g](const EdgeDescriptor ed)
    {
      Edge& edge = g[ed];
      edge.Decode();
      g[ed] = edge;
    }
  );
}

ribi::cmap::ConceptMap ribi::cmap::DotToConceptMap(const std::string& s)
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  {
    std::ofstream f(temp_filename);
    f << s;
  }
  ConceptMap c = LoadFromFile(temp_filename);
  FileIo().DeleteFile(temp_filename);
  return c;
}

ribi::cmap::VertexDescriptor ribi::cmap::FindCenterNode(const ConceptMap& g)
{
  if (CountCenterNodes(g) != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot find the one center node, as there are "
      << CountCenterNodes(g)
    ;
    throw std::invalid_argument(msg.str());
  }
  using vd = VertexDescriptor;
  const auto vip = vertices(g);
  const auto i = std::find_if(
    vip.first, vip.second,
    [g](const vd d) {
      return IsCenterNode(g[d]);
    }
  );
  assert(i != vip.second);
  return *i;
}

ribi::cmap::Node ribi::cmap::GetCenterNode(const ConceptMap& c)
{
  if (CountCenterNodes(c) != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot get the one center node, as there are "
      << CountCenterNodes(c)
    ;
    throw std::invalid_argument(msg.str());
  }
  return GetNode(FindCenterNode(c), c);
}

ribi::cmap::Edge ribi::cmap::GetEdge(
  const ribi::cmap::EdgeDescriptor ed, const ribi::cmap::ConceptMap& g) noexcept
{
  return get_my_bundled_edge(ed, g);
}

std::vector<ribi::cmap::Edge> ribi::cmap::GetEdges(const ConceptMap& c) noexcept
{
  //return get_my_bundled_edges(c);
  const auto eip = edges(c);
  std::vector<Edge> v(boost::num_edges(c));
  std::transform(eip.first,eip.second,std::begin(v),
    [c](const EdgeDescriptor& d)
    {
      return c[d];
    }
  );
  return v;
}

ribi::cmap::Edge ribi::cmap::GetFirstEdge(const ConceptMap& c)
{
  if (boost::num_edges(c) == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot get the first edge if there are zero edges"
    ;
    throw std::invalid_argument(msg.str());
  }
  return GetEdge(*edges(c).first, c);
}

ribi::cmap::Node ribi::cmap::GetFirstNode(const ConceptMap& c)
{
  if (boost::num_vertices(c) == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot get the first node if there are zero nodes"
    ;
    throw std::invalid_argument(msg.str());
  }
  return GetNode(*vertices(c).first, c);
}

std::string ribi::cmap::GetFocusName(
  const ConceptMap& sub_conceptmap)
{
  if (boost::num_vertices(sub_conceptmap) == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": cannot get the name of a focal node, "
      << "when there are zero nodes"
    ;
    throw std::invalid_argument(msg.str());
  }
  const Concept focal_concept(ribi::cmap::GetFirstNode(sub_conceptmap).GetConcept());
  return focal_concept.GetName();
}

ribi::cmap::Node ribi::cmap::GetFrom(const Edge& edge, const ConceptMap& c)
{
  return GetFrom(::find_first_bundled_edge_with_my_edge(edge, c), c);
}

ribi::cmap::Node ribi::cmap::GetFrom(const EdgeDescriptor ed, const ConceptMap& c) noexcept
{
  return GetNode(boost::source(ed, c), c);
}

std::pair<const ribi::cmap::Node&, const ribi::cmap::Node&>
ribi::cmap::GetFromTo(
  const EdgeDescriptor ed, const ConceptMap& conceptmap
)
{
  assert(boost::num_edges(conceptmap));
  const VertexDescriptor vd_from = boost::source(ed, conceptmap);
  const VertexDescriptor vd_to = boost::target(ed, conceptmap);
  assert(vd_from != vd_to);
  const Node& from = conceptmap[vd_from];
  const Node& to = conceptmap[vd_to];
  assert(from.GetId() != to.GetId());
  return { from, to };
}

const ribi::cmap::Node& ribi::cmap::GetNode(
  const ribi::cmap::VertexDescriptor vd, const ribi::cmap::ConceptMap& g
) noexcept
{
  return g[vd];
}

std::vector<ribi::cmap::Node> ribi::cmap::GetNodes(const ConceptMap& c) noexcept
{
  const auto vip = vertices(c);
  std::vector<Node> v(boost::num_vertices(c));
  std::transform(vip.first,vip.second,std::begin(v),
    [&c](const VertexDescriptor& d)
    {
      return c[d];
    }
  );
  return v;
}

std::vector<ribi::cmap::Edge> ribi::cmap::GetSortedEdges(const ConceptMap& c) noexcept
{
  auto v = GetEdges(c);
  std::sort(std::begin(v),std::end(v));
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::GetSortedNodes(const ConceptMap& c) noexcept
{
  auto v = GetNodes(c);
  std::sort(std::begin(v),std::end(v));
  return v;
}

ribi::cmap::Node ribi::cmap::GetTo(const Edge& edge, const ConceptMap& c)
{
  return GetTo(::find_first_bundled_edge_with_my_edge(edge, c), c);
}

ribi::cmap::Node ribi::cmap::GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept
{
  return GetNode(boost::target(ed, c), c);
}

bool ribi::cmap::HasCenterNode(const ConceptMap& c) noexcept
{
  const auto nodes = GetNodes(c);
  const auto i = std::find_if(
    std::begin(nodes),std::end(nodes),
    [](const Node& node) {
      return IsCenterNode(node);
    }
  );
  return i != std::end(nodes);
}

bool ribi::cmap::HasSameData(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  //Still imperfect
  return
    boost::isomorphism(lhs, rhs)
    && HasSameData(GetSortedNodes(lhs), GetSortedNodes(rhs))
    && HasSameData(GetSortedEdges(lhs), GetSortedEdges(rhs))
  ;
}

bool ribi::cmap::HasSimilarData(
  const ConceptMap& lhs,
  const ConceptMap& rhs,
  const double tolerance) noexcept
{
  //Still imperfect
  return
    boost::isomorphism(lhs, rhs)
    && HasSimilarData(GetSortedNodes(lhs), GetSortedNodes(rhs), tolerance)
    && HasSimilarData(GetSortedEdges(lhs), GetSortedEdges(rhs), tolerance)
  ;
}

bool ribi::cmap::HasUninitializedExamples(const ConceptMap& c) noexcept
{
  const auto m = TallyCompetencies(c);
  return m.count(Competency::uninitialized);
}

bool ribi::cmap::IsConnectedTo(const Edge& edge, const Node& node, const ConceptMap& c)
{
  return GetFrom(edge,c) == node || GetTo(edge, c) == node;
}

ribi::cmap::ConceptMap ribi::cmap::LoadFromFile(const std::string& dot_filename)
{
  //auto g = load_undirected_bundled_edges_and_vertices_graph_from_dot<
  //    decltype(ConceptMap())
  //  >(dot_filename)
  //;
  if (!is_regular_file(dot_filename))
  {
    std::stringstream msg;
    msg << __func__ << ": file '"
      << dot_filename << "' not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  std::ifstream f(dot_filename);
  ConceptMap g;
  boost::dynamic_properties dp(boost::ignore_other_properties);
  dp.property("label", get(boost::vertex_bundle, g));
  dp.property("label", get(boost::edge_bundle, g));
  boost::read_graphviz(f, g, dp);


  DecodeConceptMap(g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::RemoveFirstNode(ConceptMap g)
{
  if (!boost::num_vertices(g))
  {
    throw std::invalid_argument("RemoveFirstNode: need at least one node");
  }
  boost::clear_vertex(*vertices(g).first, g);
  boost::remove_vertex(*vertices(g).first, g);
  return g;
}

void ribi::cmap::SaveToFile(const ConceptMap& g, const std::string& dot_filename)
{
  //save_bundled_edges_and_vertices_graph_to_dot(g, dot_filename);
  //Cannot use:
  //save_bundled_edges_and_vertices_graph_to_dot(g, dot_filename);
  //because we have a directed graph in which edegs have zero/one/two arrow heads
  std::ofstream f(dot_filename);
  //boost::write_graphviz(f, g);
  boost::write_graphviz(f, g,
    ::ribi::cmap::make_node_writer(g),
    ::ribi::cmap::make_edge_writer(g)
  );
}

void ribi::cmap::SaveToImage(const ConceptMap& g, const std::string& png_filename)
{
  const std::string dot_filename{"SaveToImage.dot"};
  const std::string svg_filename{"SaveToImage.svg"};
  SaveToFile(g, dot_filename);
  convert_dot_to_svg(dot_filename, svg_filename);
  convert_svg_to_png(svg_filename, png_filename);
  ribi::FileIo().DeleteFile(dot_filename);
  ribi::FileIo().DeleteFile(svg_filename);
}

void ribi::cmap::SaveSummaryToImage(const ConceptMap& g, const std::string& png_filename)
{
  const std::string dot_filename{"SaveToImage.dot"};
  const std::string svg_filename{"SaveToImage.svg"};
  SaveSummaryToFile(g, dot_filename);
  convert_dot_to_svg(dot_filename, svg_filename);
  convert_svg_to_png(svg_filename, png_filename);
  ribi::FileIo().DeleteFile(dot_filename);
  ribi::FileIo().DeleteFile(svg_filename);
}

void ribi::cmap::SaveSummaryToFile(const ConceptMap& g, const std::string& dot_filename)
{
  std::ofstream f(dot_filename);

  boost::write_graphviz(f, g,
    [g](std::ostream& out, const VertexDescriptor& vd) {
      out << "[label=\""
        << GetName(g[vd].GetConcept())
        << "\"]"
      ;
    },
    [g](std::ostream& out, const EdgeDescriptor& ed) {
      const auto& edge = g[ed];
      out << "[label=\""
        << edge.GetNode().GetConcept().GetName()
        << "\", "
       ;

      const bool has_head{edge.HasHeadArrow()};
      const bool has_tail{edge.HasTailArrow()};
      if ( has_head &&  has_tail)
      {
        out << "dir = \"both\", arrowhead = \"normal\", arrowtail = \"normal\"";
      }
      if ( has_head && !has_tail)
      {
        out << "dir = \"forward\", arrowhead = \"normal\"";
      }
      if (!has_head &&  has_tail)
      {
        out << "dir = \"back\", arrowtail = \"normal\"";
      }
      out << "]";
    }
  );
}

std::map<ribi::cmap::Competency,int> ribi::cmap::TallyCompetencies(
  const ConceptMap& g
) noexcept
{
  const std::vector<Competency> competencies = CollectCompetenies(g);
  return CreateTally(competencies);
}

std::string ribi::cmap::ToDot(const ConceptMap& g) noexcept
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  SaveToFile(g, temp_filename);
  std::stringstream s;
  s << FileIo().FileToStr(temp_filename);
  FileIo().DeleteFile(temp_filename);
  return s.str();
}

std::string ribi::cmap::ToXml(const ConceptMap& conceptmap) noexcept
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  SaveToFile(conceptmap, temp_filename);
  std::stringstream s;
  s << "<conceptmap>"
    << FileIo().FileToStr(temp_filename) // << ToDot(conceptmap);
    << "</conceptmap>"
  ;
  FileIo().DeleteFile(temp_filename);
  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,12) == "<conceptmap>");
  assert(r.substr(r.size() - 13,13) == "</conceptmap>");
  return r;
}

ribi::cmap::ConceptMap ribi::cmap::XmlToConceptMap(const std::string& s)
{
 if (s.size() < 13)
  {
    std::stringstream msg;
    msg << __func__ << ": string too short, "
      << "received '" << s << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  const std::string required_tag{"<conceptmap>"};
  const int required_tag_size{static_cast<int>(required_tag.size())};
  if (s.substr(0, required_tag_size) != required_tag)
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect starting tag, "
      << "required '" << required_tag << "', "
      << "received '" << s << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  if (s.substr(s.size() - 13,13) != "</conceptmap>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect ending tag";
    throw std::invalid_argument(msg.str());
  }
  //Need to write the DOT to file
  const std::string dot_filename = ribi::fileio::FileIo().GetTempFileName(".dot");
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConceptMap());
    assert(v.size() == 1);
    const std::string dot_str{
      ribi::xml::StripXmlTag(v[0])
    };

    std::ofstream f(dot_filename);
    f << dot_str;
  }
  ConceptMap conceptmap = LoadFromFile(dot_filename);
  ribi::fileio::FileIo().DeleteFile(dot_filename);
  return conceptmap;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept
{
  os << graphviz_encode(ToXml(conceptmap));
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, ConceptMap& conceptmap)
{
  //eat until '</conceptmap>'
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 13 && s.substr(s.size() - 13,13) == "</conceptmap>") break;
    assert(s != "00000");
  }
  conceptmap = XmlToConceptMap(graphviz_decode(s));
  return is;
}

bool ribi::cmap::operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  //Still imperfect
  return
    boost::isomorphism(lhs, rhs)
    && GetSortedNodes(lhs) == GetSortedNodes(rhs)
    && GetSortedEdges(lhs) == GetSortedEdges(rhs)
  ;
}

bool ribi::cmap::operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  return !(lhs == rhs);
}
