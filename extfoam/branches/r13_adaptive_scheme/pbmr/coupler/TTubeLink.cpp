
#include <TTubeLink.h>
#include <TQueue.h>

namespace Impl {

TTubeLink::TTubeLink()
{
  m_Queue = Base::TQueuePtr( new TQueue() );
}

void TTubeLink::write( Base::TFieldPtr theField )
{
  m_Queue->write( theField );
}

Base::TFieldPtr TTubeLink::read()
{
  return m_Queue->read();
}

Base::TFieldPtr TTubeLink::read_domain( int theSubDomain )
{
  return Base::TFieldPtr();
}

}
