#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "conceptmapexamplesfactory.h"

#include <cassert>
#include <vector>
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapregex.h"
#pragma GCC diagnostic pop

ribi::cmap::ExamplesFactory::ExamplesFactory() noexcept
{

}

ribi::cmap::Examples ribi::cmap::ExamplesFactory::Create(
  const std::vector<std::pair<std::string,Competency> >& v) const noexcept
{
  std::vector<Example> w;
  std::transform(v.begin(),v.end(),std::back_inserter(w),
    [](const std::pair<std::string,Competency>& p)
    {
      const Example q
       (p.first,p.second);
      return q;
    }
  );
  Examples q(w);
  return q;
}

ribi::cmap::Examples ribi::cmap::ExamplesFactory::GetTest(const int i) const noexcept
{
  assert(i >= 0);
  assert(i < GetNumberOfTests());
  return GetTests()[i];
}

ribi::cmap::Examples ribi::cmap::ExamplesFactory::Get0() const noexcept
{
  assert(!GetTests().empty());
  return GetTests()[0];
}

std::vector<ribi::cmap::Examples> ribi::cmap::ExamplesFactory::GetNastyTests() const noexcept
{
  std::vector<Examples> v;
  v.push_back(Examples(ExampleFactory().GetNastyTests()));
  return v;
}

std::vector<ribi::cmap::Examples> ribi::cmap::ExamplesFactory::GetTests() const noexcept
{
  const std::vector<std::vector<int> > is = { {0}, {1}, {0,1,2,3}, {} };
  std::vector<Examples> v;
  std::transform(is.begin(),is.end(),std::back_inserter(v),
    [this](const std::vector<int>& js)
    {
      std::vector<Example> w;
      std::transform(js.begin(),js.end(),std::back_inserter(w),
        [](const int& j)
        {
          const Example p = ExampleFactory().GetTest(j);
          return p;
        }
      );
      const Examples q(w);
      return q;
    }
  );

  return v;

}

