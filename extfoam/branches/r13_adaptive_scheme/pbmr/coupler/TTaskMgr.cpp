
#include <TTaskMgr.h>

#include <TCableLink.h>
#include <TCollectorLink.h>
#include <TDiffusorLink.h>
#include <TTubeLink.h>

#include <iostream>
#include <algorithm>

namespace Impl {

TTaskMgr::TTaskMgr()
: m_IsRun( false )
{
}

TTaskMgr::TPtr TTaskMgr::get_instance()
{
  static TPtr anInst = Base::HTaskMgr< Impl::TTaskMgr >::create( new TTaskMgr() );
  
  return anInst;
}

void* thrd_start( void* ptr )
{
  Base::TTask* aTask = (Base::TTask*)ptr;
  if( aTask )
    aTask->invoke( TTaskMgr::get_instance() );
  return 0;
}

void TTaskMgr::start()
{
  if( m_Tasks.size()==0 || m_IsRun )
    return;

  m_IsRun = true;
  TaskListIt it = m_Tasks.begin(), last = m_Tasks.end();
  for( ; it!=last; it++ )
  {
    pthread_t thrd;
    pthread_create( &thrd, NULL, thrd_start, (*it).operator->() );
  }
}

void TTaskMgr::stop()
{
  m_IsRun = false;
}

void TTaskMgr::pause()
{
  pthread_mutex_lock( &m_Pause );
}

void TTaskMgr::resume()
{
  pthread_mutex_unlock( &m_Pause );
}

bool TTaskMgr::is_run()
{
  pthread_mutex_lock( &m_Pause );
  pthread_mutex_unlock( &m_Pause );
  return m_IsRun;
}

void TTaskMgr::add_task( Base::TTaskPtr theTask )
{
  if( find( m_Tasks.begin(), m_Tasks.end(), theTask ) == m_Tasks.end() )
    m_Tasks.push_back( theTask );
}

void TTaskMgr::connect( Base::TTaskPtr theSource, const std::string& theSourceName,
                        Base::TTaskPtr theDest, const std::string& theDestName )
{
  if( !theSource || !theDest )
    return;

  bool
    isSMult = theSource->is_multi_subdomain(),
    isDMult = theDest->is_multi_subdomain();
    
  Base::TLinkPtr aLink;
  if( isSMult )
    if( isDMult ) {
      aLink = new TCableLink();
      //aLink = m_link_factory->create_cable_link();
    } else {
      aLink = new TCollectorLink( m_Domain );
      //aLink = m_link_factory->create_collector_link( m_Domain );
    }
  else {
    if( isDMult ) {
      aLink = new TDiffusorLink( m_Domain );
      //aLink = m_link_factory->create_diffusor_link( m_Domain );
    } else {
      aLink = new TTubeLink();
      //aLink = m_link_factory->create_tube_link();
    }
  }
  theSource->add_link( aLink, theSourceName, false );
  theDest->add_link( aLink, theDestName, true );
}

Base::TDomainPtr TTaskMgr::get_domain()
{
  return m_Domain;
}

void TTaskMgr::set_domain( Base::TDomainPtr theDomain )
{
  m_Domain = theDomain;
}

void TTaskMgr::set_link_factory( Base::TLinkFactoryPtr the_link_factory )
{
  m_link_factory = the_link_factory;
}

}
