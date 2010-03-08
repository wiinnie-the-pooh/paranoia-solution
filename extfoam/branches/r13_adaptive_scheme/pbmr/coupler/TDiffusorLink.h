
#ifndef DIFFUSOR_LINK_HEADER
#define DIFFUSOR_LINK_HEADER

#include <Interfaces.h>
#include <TQueueContainer.h>

namespace Impl
{
  class TDiffusorLink : public TQueueContainer
  {
  public:
    TDiffusorLink( Base::TDomainPtr theDomain );

    virtual void write( Base::TFieldPtr theField );
    virtual Base::TFieldPtr read();
    virtual Base::TFieldPtr read_domain( int theSubDomain );

  private:
    Base::TDomainPtr m_Domain;
  };
}

#endif
