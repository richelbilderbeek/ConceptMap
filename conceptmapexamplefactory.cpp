#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapexamplefactory.h"

#include <cassert>

#include "conceptmapcompetencies.h"
#include "conceptmaphelper.h"
#include "conceptmapexample.h"
#include "conceptmapregex.h"
#pragma GCC diagnostic pop

ribi::cmap::ExampleFactory::ExampleFactory() noexcept
{

}

ribi::cmap::Example ribi::cmap::ExampleFactory::GetTest(const int i) const noexcept
{
  assert(i < GetNumberOfTests());
  return GetTests()[i];
}

std::vector<ribi::cmap::Example> ribi::cmap::ExampleFactory::GetNastyTests() const noexcept
{
  return
  {
    Example(" Test example 0",Competency::profession,true,false,false),
    Example("Test>example 1",Competency::organisations,false,true,false),
    Example("Test example<2",Competency::social_surroundings,false,false,true)
  };
}

std::vector<ribi::cmap::Example> ribi::cmap::ExampleFactory::GetTests() const noexcept
{
  return
  {
    Example("Test example 0",Competency::profession,true,false,false),
    Example("Test example 1",Competency::organisations,false,true,false),
    Example("Test example 2",Competency::social_surroundings,false,false,true),
    Example("Test example 3",Competency::target_audience,true,true,false),
    Example("Test example 4",Competency::ti_knowledge,false,true,true),
    Example("Test example 5",Competency::prof_growth,true,false,true),
    Example("Test example 6",Competency::misc,true,true,true),
    Example("",Competency::uninitialized,true,false,true)
  };
}

