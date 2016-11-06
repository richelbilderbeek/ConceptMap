#ifndef CONCEPTMAPCOMPETENCIES_H
#define CONCEPTMAPCOMPETENCIES_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/bimap.hpp>
#include "conceptmapcompetency.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Functions to work on with the Competency enumeration
struct Competencies
{
  Competencies();
  std::vector<Competency> GetAllCompetencies() const noexcept;
  int ToIndex(const Competency competency) const noexcept;
  std::string ToStrDutch(const Competency competency) const noexcept;
  std::string ToStr(const Competency competency) const noexcept;
  Competency ToTypeFromDutch(const std::string& dutch_string) const;
  Competency ToType(const std::string& s) const;

  private:

  mutable boost::bimap<Competency,std::string> m_map_dutch; //Lazy initialization
  mutable boost::bimap<Competency,std::string> m_map_english; //Lazy initialization
};

boost::bimap<Competency,std::string> CreateMapDutch() noexcept;
boost::bimap<Competency,std::string> CreateMapEnglish() noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMPETENCIES_H
