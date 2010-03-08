
#include <TDiffusorLink.h>
#include <TField.h>

namespace Impl {

TDiffusorLink::TDiffusorLink( Base::TDomainPtr theDomain )
: m_Domain( theDomain )
{
}

void TDiffusorLink::write( Base::TFieldPtr theField )
{
  if( theField->get_subdomain()>=0 )
    return;

  Base::TDataItem* aDataPtr = (Base::TDataItem*)theField->get_raw_data();

  std::list<int> aSubDomains = m_Domain->get_subdomains();
  std::list<int>::const_iterator anIt = aSubDomains.begin(), aLast = aSubDomains.end();
  for( ; anIt!=aLast; anIt++ )
  {
    int aSubDomain = *anIt;

    std::list<int> anInds = m_Domain->get_subdomain_indices( aSubDomain );
    std::list<int>::const_iterator anIndIt = anInds.begin(), anIndLast = anInds.end();

    Base::TDataItem* aSubData = new Base::TDataItem[anInds.size()];
    for( int i=0; anIndIt!=anIndLast; anIndIt++, i++ )
      aSubData[i] = aDataPtr[*anIndIt];

    int aSize = sizeof(Base::TDataItem) * anInds.size();
    Base::TFieldPtr aNewField( new TField( theField->get_name(), aSubDomain, aSubData, aSize ) );
    get_queue( aSubDomain )->write( aNewField );
    delete[] aSubData;
  }
}

Base::TFieldPtr TDiffusorLink::read()
{
  return Base::TFieldPtr();
}

Base::TFieldPtr TDiffusorLink::read_domain( int theSubDomain )
{
  return get_queue( theSubDomain )->read();
}

}
