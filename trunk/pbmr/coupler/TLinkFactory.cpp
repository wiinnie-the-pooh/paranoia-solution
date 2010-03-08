
#include <TLinkFactory.h>
#include <Holders.h>

#include <TCableLink.h>
#include <TCollectorLink.h>
#include <TDiffusorLink.h>
#include <TTubeLink.h>


//---------------------------------------------------------------------------
using namespace Base;


//---------------------------------------------------------------------------
TLinkPtr Impl::TLinkFactory::create_cable_link()
{
  return HLink<>::create( new TCableLink() );
}


//---------------------------------------------------------------------------
TLinkPtr Impl::TLinkFactory::create_collector_link( const TDomainPtr& the_domain )
{
  return HLink<>::create( new TCollectorLink( the_domain ) );
}


//---------------------------------------------------------------------------
TLinkPtr Impl::TLinkFactory::create_diffusor_link( const TDomainPtr& the_domain )
{
  return HLink<>::create( new TDiffusorLink( the_domain ) );
}


//---------------------------------------------------------------------------
TLinkPtr Impl::TLinkFactory::create_tube_link()
{
  return HLink<>::create( new TTubeLink() );
}


//---------------------------------------------------------------------------
