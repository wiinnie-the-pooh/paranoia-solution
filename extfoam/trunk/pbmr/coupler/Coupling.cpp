
#include "Coupling.h"

#include <TTaskMgr.h>
#include <SolverExamples.h>
#include <TLinkFactory.h>

#include <iostream>

#define MESSAGES

const std::string START_TASK = "START", END_TASK = "END";

std::map<std::string, Base::TTaskPtr> Coupling::m_Tasks = std::map<std::string, Base::TTaskPtr>();
std::list<CppLinkInfo> Coupling::m_StartTasks = std::list<CppLinkInfo>();

void MESSAGE( const std::string& msg )
{
  std::cout << msg << std::endl;
}

Coupling::Coupling()
{
  using namespace Base;

  HTaskMgr< Impl::TTaskMgr >::TPtr aMgr = Impl::TTaskMgr::get_instance();
  
  HLinkFactory< Impl::TLinkFactory >::TPtr a_link_factory =
      HLinkFactory< Impl::TLinkFactory >::create( new Impl::TLinkFactory() );

  aMgr->device()->set_link_factory( a_link_factory );
}

Coupling::~Coupling()
{
}

bool Coupling::create_task( const std::string& theName, Base::TTaskPtr theTask )
{
  if( m_Tasks.find( END_TASK ) != m_Tasks.end() )
    return false;

#ifdef MESSAGES
  std::string msg = "Task ";
  msg += theName;
  MESSAGE( msg );
#endif

  Base::TTaskMgrPtr aMgr = Impl::TTaskMgr::get_instance();
  if( theTask )
    aMgr->add_task( theTask );
  m_Tasks[theName] = theTask;
  return true;
}

void Coupling::create_input( const std::string& theTaskName, const std::string& thePortName, const CppLinkInfo& thePortData )
{
  if( thePortData.m_TaskName == START_TASK )
  {
    CppLinkInfo anInfo;
    anInfo.m_Port = thePortName;
    anInfo.m_TaskName = theTaskName;
    m_StartTasks.push_back( anInfo );

#ifdef MESSAGES
    std::string msg = "Start task: port = ";
    msg += thePortName;
    msg += ", task = ";
    msg += theTaskName;
    MESSAGE( msg );
#endif
  }
  else
  {
    Base::TTaskMgrPtr aMgr = Impl::TTaskMgr::get_instance();
    Base::TTaskPtr aTask = m_Tasks[theTaskName];
    aMgr->connect( m_Tasks[thePortData.m_TaskName],
                   thePortData.m_Port, aTask, thePortName );

#ifdef MESSAGES
    std::string msg = "Port ";
    msg += thePortName;
    msg += " of task ";
    msg += theTaskName;
    msg += " is connected from [";
    msg += thePortData.m_TaskName;
    msg += ", ";
    msg += thePortData.m_Port;
    msg += "]";
    
    MESSAGE( msg );
#endif
  }
}

void Coupling::create_output( const std::string& theTaskName, const std::string& thePortName, CppLinkInfo& theData )
{
#ifdef MESSAGES
  std::string msg = "Output port information [";
  msg += thePortName;
  msg += ", ";
  msg += theTaskName;
  msg += "]";
  MESSAGE( msg );
#endif

  theData.m_Port = thePortName;
  theData.m_TaskName = theTaskName;
}



void Coupling::START( CppLinkInfo& res )
{
#ifdef MESSAGES
  MESSAGE( "Coupling::START" );
#endif
  if( m_Tasks.find( END_TASK ) == m_Tasks.end() )
  {
    std::string aName = START_TASK;
    m_Tasks[aName] = Base::TTaskPtr();
    create_output( START_TASK, "res", res );
  }
  else
  {
    Impl::TTaskMgr::TPtr aMgr = Impl::TTaskMgr::get_instance();
    aMgr->device()->set_domain( Base::TDomainPtr( new Examples::ArrayDomain() ) );
    aMgr->start();
  }
}

void Coupling::plus_x( const CppLinkInfo& arg, CppLinkInfo& res )
{
  if( !create_task( "PLUS", new Examples::PlusX( 1 ) ) )
    return;

#ifdef MESSAGES
  MESSAGE( "Coupling::PLUS_X" );
#endif
  create_input( "PLUS", "arg", arg );
  create_output( "PLUS", "res", res );
}

void Coupling::mult_x( const CppLinkInfo& arg, CppLinkInfo& res )
{
  if( !create_task( "MULT", new Examples::MultX( 2 ) ) )
    return;

#ifdef MESSAGES
  MESSAGE( "Coupling::MULT_X" );
#endif
  create_input( "MULT", "arg", arg );
  create_output( "MULT", "res", res );
}

void Coupling::array_vis( const CppLinkInfo& arg )
{
  if( !create_task( "VIS", new Examples::ArrayVis( "arrayvis" ) ) )
    return;

#ifdef MESSAGES
  MESSAGE( "Coupling::VIS" );
#endif
  create_input( "VIS", "arg", arg );
}

void Coupling::END( const CppLinkInfo& arg )
{
  if( !create_task( END_TASK, Base::TTaskPtr() ) )
    return;

#ifdef MESSAGES
  char buf[100];
  sprintf( buf, "size = %i", m_StartTasks.size() );
  MESSAGE( buf );
#endif

  std::list<CppLinkInfo>::const_iterator it = m_StartTasks.begin(), last = m_StartTasks.end();
  for( ; it!=last; it++ )
    create_input( it->m_TaskName, it->m_Port, arg );
}
