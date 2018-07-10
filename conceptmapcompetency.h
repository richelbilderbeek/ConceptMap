#ifndef CONCEPTMAPCOMPETENCY_H
#define CONCEPTMAPCOMPETENCY_H

#include <iosfwd>

namespace ribi {
namespace cmap {

enum class Competency
{
  uninitialized,       //Not yet set, must equal zero
  profession,          //NL: 'Beroepsdomein'
  organisations,       //NL: 'Organisaties'
  social_surroundings, //NL: 'Sociale omgeving'
  target_audience,     //NL 'Doelgroep'
  ti_knowledge,        //'Technical Instrumental', NL: 'Technische instrumentele kennis'
  prof_growth,         //Professionele groei
  misc,                //NL: 'Overig'
  n_competencies       //Used for debugging only
};

std::string CompetencyToStr(const Competency competency) noexcept;
std::string CompetencyToStrDutch(const Competency competency) noexcept;
Competency StrToCompetency(const std::string s);
std::ostream& operator<<(std::ostream& os, const Competency competency);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMPETENCY_H
