
#include <TCableLink.h>

namespace Impl {

void TCableLink::write( Base::TFieldPtr theField )
{
  if( theField )
    get_queue( theField->get_subdomain() )->write( theField );
}

Base::TFieldPtr TCableLink::read_domain( int theSubDomain )
{
  return get_queue( theSubDomain )->read();
}

Base::TFieldPtr TCableLink::read()
{
  return Base::TFieldPtr();
}

}
