
#include <TCollectorLink.h>
#include <TQueue.h>
#include <TField.h>
#include <iostream>

namespace Impl {

TCollectorLink::TCollectorLink( Base::TDomainPtr theDomain )
: m_Domain( theDomain )
{
  m_Queue = Base::TQueuePtr( new TQueue() );
  m_Items = new Base::TDataItem[m_Domain->get_count()];
}

TCollectorLink::~TCollectorLink()
{
  delete[] m_Items;
}

void TCollectorLink::write( Base::TFieldPtr theField )
{
  int aSubDomain = theField->get_subdomain();
  //printf( "TCollectorLink::write: subdomain = %i\n", aSubDomain );
  Base::TDataItem* aDataPtr = (Base::TDataItem*)theField->get_raw_data();

  std::list<int> anInds = m_Domain->get_subdomain_indices( aSubDomain );
  std::list<int>::const_iterator anIt = anInds.begin(), aLast = anInds.end();
  for( int i=0; anIt!=aLast; anIt++, i++ )
  {
    //std::cout << *anIt << " ";
    m_Items[*anIt] = aDataPtr[i];
    m_Exists[*anIt] = true;
  }
  //std::cout << std::endl;
  //std::cout << "TCollectorLink's map size = " << m_Exists.size() << std::endl;
  if( m_Exists.size()==m_Domain->get_count() )
  {
    m_Queue->write( Base::TFieldPtr( new TField( theField->get_name(), m_Items, sizeof( Base::TDataItem ) * m_Exists.size() ) ) );
    m_Exists.clear();
  }
}

Base::TFieldPtr TCollectorLink::read()
{
  return m_Queue->read();
}

Base::TFieldPtr TCollectorLink::read_domain( int theSubDomain )
{
  return Base::TFieldPtr();
}

}
