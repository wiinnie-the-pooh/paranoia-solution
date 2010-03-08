
#include <TTask.h>
#include <TTaskMgr.h>
#include <TField.h>
#include <iostream>

namespace Impl {

TTask::TTask( bool theIsMultiSubDomain )
: m_This( this ), m_IsMultiSubDomain( theIsMultiSubDomain )
{
}

Base::TTaskPtr TTask::get_this()
{
  return m_This;
}

void TTask::invoke( Base::TTaskMgrPtr theMgr )
{
  TSubTask::invoke( theMgr );
}

void TTask::add_link( Base::TLinkPtr theLink, const std::string& theName, bool theIsInput )
{
  if( theIsInput )
    m_In[theName] = theLink;
  else
  {
    std::list<Base::TLinkPtr>& lst = m_Out[theName];
    if( find( lst.begin(), lst.end(), theLink ) == lst.end() )
      lst.push_back( theLink );
  }
}

Base::TFieldPtr TTask::read( const std::string& theName, int theSubDomain )
{
  Base::TFieldPtr res;
  LinkMap::const_iterator it = m_In.find( theName );
  if( it != m_In.end() )
  {
    Base::TLinkPtr aLink = (*it).second;
    if( is_multi_subdomain() )
      res = aLink->read_domain( theSubDomain );
    else
      res = aLink->read();
  }
  return res;
}

void TTask::write( const std::string& theName, Base::TFieldPtr theField )
{
  LinkListMap::const_iterator it = m_Out.find( theName );
  if( it != m_Out.end() )
  {
    const std::list<Base::TLinkPtr>& lst = (*it).second;
    std::list<Base::TLinkPtr>::const_iterator lit = lst.begin(), llast = lst.end();
    for( ; lit!=llast; lit++ )
      (*lit)->write( theField );
  }
}

void TTask::write_raw( const std::string& theName, int theSubDomain, Base::TData theData, int theSize )
{
  write( theName, Base::TFieldPtr( new TField( theName, theSubDomain, theData, theSize ) ) );
}

void TTask::init( Base::TDomainPtr theDomain )
{
}

void TTask::destroy()
{
}

void TTask::step( Base::TDomainPtr theDomain )
{
}

void* sub_task_start( void* ptr )
{
  Base::TSubTask* aTask = (Base::TSubTask*)ptr;
  if( aTask )
    aTask->invoke( TTaskMgr::get_instance() );
  return 0;
}

void TTask::start_sub_task( Base::TSubTaskPtr theSubTask )
{
  pthread_t thrd;
  pthread_create( &thrd, NULL, sub_task_start, theSubTask.operator->() );
}

bool TTask::is_multi_subdomain()
{
  return m_IsMultiSubDomain;
}

}
