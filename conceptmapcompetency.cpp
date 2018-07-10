#include "conceptmapcompetency.h"
#include "conceptmapcompetencies.h"

std::string ribi::cmap::CompetencyToStr(const Competency competency) noexcept
{
  return Competencies().ToStr(competency);
}

std::string ribi::cmap::CompetencyToStrDutch(const Competency competency) noexcept
{
  return Competencies().ToStrDutch(competency);
}

ribi::cmap::Competency ribi::cmap::StrToCompetency(const std::string s)
{
  return Competencies().ToType(s);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Competency competency)
{
  os << Competencies().ToStr(competency);
  return os;
}
