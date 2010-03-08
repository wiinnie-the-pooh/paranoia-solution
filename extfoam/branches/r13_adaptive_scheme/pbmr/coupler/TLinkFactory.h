
#ifndef LINK_FACTORY_HEADER
#define LINK_FACTORY_HEADER

#include <Interfaces.h>

namespace Impl
{
  struct TLinkFactory : virtual TGenericObj
  {
    virtual Base::TLinkPtr create_cable_link();
    virtual Base::TLinkPtr create_collector_link( const Base::TDomainPtr& the_domain );
    virtual Base::TLinkPtr create_diffusor_link( const Base::TDomainPtr& the_domain );
    virtual Base::TLinkPtr create_tube_link();
  };
}

#endif
