// VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
// Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/corba/server/TaskManager_i.hh"

#include "parallel/corba/server/Link_i.hh"

#include "parallel/corba/idl/PortBase.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace
{
  //---------------------------------------------------------------------------
  using namespace parallel;


  //---------------------------------------------------------------------------
  Link_ptr create_link( const CORBA::ORB_var& theORB, 
                        const PortableServer::POA_var& thePOA )
  {
    Link_i* an_item( new Link_i( theORB, thePOA ) );

    return an_item->_this();
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskManager_i::TaskManager_i( const CORBA::ORB_var& theORB, 
                                const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , m_is_run( false )
  {
    cout << "TaskManager_i::TaskManager_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  TaskManager_i::~TaskManager_i()
  {
    cout << "TaskManager_i::~TaskManager_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean TaskManager_i::connect( TaskBase_ptr theSourceTask, 
                                         const char* theOutputPortName,
                                         TaskBase_ptr theTargetTask, 
                                         const char* theInputPortName )
  {
    cout << "TaskManager_i::connect[ " << this << " ] : '" << theOutputPortName << "'; '" << theInputPortName << "'" << endl;

    if ( CORBA::is_nil( theSourceTask ) || CORBA::is_nil( theTargetTask ) )
      return false;

    Link_var a_link( ::create_link( this->ORB(), this->POA() ) );

    PortBase_var an_output_port = theSourceTask->get_output_port( theOutputPortName );
    if ( CORBA::is_nil( an_output_port ) )
      return false;

    PortBase_var an_input_port = theTargetTask->get_input_port( theInputPortName );
    if ( CORBA::is_nil( an_input_port ) )
      return false;

    if ( !theSourceTask->connect_output( an_output_port, a_link, an_input_port ) )
      return false;

    if ( !theTargetTask->connect_input( an_input_port, a_link, an_output_port ) )
      return false;

    a_link->Release();

    this->register_task( theSourceTask );
    this->register_task( theTargetTask );

    return true;
  }
    
    
  //---------------------------------------------------------------------------
  void TaskManager_i::register_task( TaskBase_ptr theTask )
  {
    if ( CORBA::is_nil( theTask ) )
      return;

    theTask->AddRef();
    TTaskBasePtr aTask( TaskBase::_duplicate( theTask ) );

    if ( this->tasks.find( aTask ) == this->tasks.end() )
    {
      cout << "TaskManager_i::register_task[ " << this << " ]" << endl;

      this->tasks[ aTask ];
    }
  }
    
    
  //---------------------------------------------------------------------------
  struct STaskInvoker : omni_thread
  {
    STaskInvoker( const TaskManager_var& theTaskManager, 
		  const TaskManager_i::TTaskBasePtr& theTask )
      : omni_thread( NULL, PRIORITY_NORMAL )
      , m_task_manager( theTaskManager )
      , m_task( theTask )
    {
      this->start_undetached();
    }

  protected:
    virtual void* run_undetached( void* /*arg*/ ) 
    { 
      m_task->invoke( m_task_manager );
      
      return NULL; 
    }

    TaskManager_var m_task_manager;
    TaskManager_i::TTaskBasePtr m_task;
  };


  //---------------------------------------------------------------------------
  void TaskManager_i::start()
  {
    cout << "TaskManager_i::start[ " << this << " ]" << endl;
    
    if ( this->is_run() )
      this->wait();

    this->m_is_run = true;

    // To prepare all the tasks first
    {
      TTaskSet::const_iterator anIter = this->tasks.begin();
      TTaskSet::const_iterator anEnd = this->tasks.end();
      for ( ; anIter != anEnd; anIter++ )
      {
        const TTaskBasePtr& aTaskBase = anIter->first;
        aTaskBase->prepare();
      }
    }

    // Invoke then
    TaskManager_var aSelf = this->_this();
    {
      TTaskSet::iterator anIter = this->tasks.begin();
      TTaskSet::iterator anEnd = this->tasks.end();
      for ( ; anIter != anEnd; anIter++ )
      {
	const TTaskBasePtr& aTaskBase = anIter->first;
        anIter->second = new STaskInvoker( aSelf, aTaskBase );
      }
    }
  }
    
    
  //---------------------------------------------------------------------------
  void TaskManager_i::wait()
  {
    TTaskSet::const_iterator anIter = this->tasks.begin();
    TTaskSet::const_iterator anEnd = this->tasks.end();
    for ( ; anIter != anEnd; anIter++ )
    {
      anIter->second->join( NULL );
    }
  }


  //---------------------------------------------------------------------------
  void TaskManager_i::stop()
  {
    this->m_is_run = false;
  }


  //---------------------------------------------------------------------------
  void TaskManager_i::pause()
  {
    this->m_pause_mutex.lock();
  }


  //---------------------------------------------------------------------------
  void TaskManager_i::resume()
  {
    this->m_pause_mutex.unlock();
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean TaskManager_i::is_run()
  {
    this->m_pause_mutex.lock();
    this->m_pause_mutex.unlock();

    return this->m_is_run;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
