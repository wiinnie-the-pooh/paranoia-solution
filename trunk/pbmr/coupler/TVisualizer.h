
#ifndef VISUALIZER_HEADER
#define VISUALIZER_HEADER

#include <TTask.h>

namespace Impl
{
  class TVisualizer : public Impl::TTask
  {
  public:
    TVisualizer( bool theIsMultiSubDomain );

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
  };
}

#endif
