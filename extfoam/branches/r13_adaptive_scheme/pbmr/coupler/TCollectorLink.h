
#ifndef COLLECTOR_LINK_HEADER
#define COLLECTOR_LINK_HEADER

#include <Interfaces.h>

namespace Impl
{
  class TCollectorLink : public Base::TLink
  {
  public:
    TCollectorLink( Base::TDomainPtr theDomain );
    virtual ~TCollectorLink();

    virtual void write( Base::TFieldPtr theField );
    virtual Base::TFieldPtr read();
    virtual Base::TFieldPtr read_domain( int theSubDomain );

  private:
    Base::TDomainPtr m_Domain;
    Base::TQueuePtr m_Queue;
    Base::TDataItem* m_Items;
    std::map<int, bool> m_Exists;
  };
}

#endif
