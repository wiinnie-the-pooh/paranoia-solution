
#include <TSubTask.h>
#include <TField.h>

namespace Impl {

TSubTask::TSubTask( int theSubDomain, Base::TTaskPtr theParent )
  : m_SubDomain( theSubDomain ), m_Parent( theParent )
{
}

void TSubTask::invoke( Base::TTaskMgrPtr theMgr )
{
  Base::TDomainPtr aDomain = theMgr->get_domain();
  init( aDomain );
  while( true )
  {
    if( theMgr->is_run() )
      step( aDomain );
    else
    {
      destroy();
      break;
    }
  }
}

Base::TFieldPtr TSubTask::read( const std::string& theName )
{
  return m_Parent->read( theName, m_SubDomain );
}

void TSubTask::write( const std::string& theName, Base::TData theData, int theSize )
{
  m_Parent->write( theName, Base::TFieldPtr( new TField( theName, m_SubDomain, theData, theSize ) ) );
}

int TSubTask::get_subdomain()
{
  return m_SubDomain;
}

}
