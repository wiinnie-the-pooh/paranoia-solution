
#ifndef CABLE_LINK_HEADER
#define CABLE_LINK_HEADER

#include <Interfaces.h>
#include <TQueueContainer.h>

namespace Impl
{
  class TCableLink : public TQueueContainer
  {
  public:
    virtual void write( Base::TFieldPtr theField );
    virtual Base::TFieldPtr read();
    virtual Base::TFieldPtr read_domain( int theSubDomain );
  };
}

#endif
