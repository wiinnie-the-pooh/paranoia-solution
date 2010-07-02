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
#ifndef corba_server_TaskFactoryArgsBase_i_hh
#define corba_server_TaskFactoryArgsBase_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/SObjectBase.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include <omnithread.h>

//---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__
#include "parallel/corba/common/TaskLauncher.hh"
#endif


//---------------------------------------------------------------------------
#include <queue>
#include <string>

#include <iostream>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
#ifndef __USE_CORBA_SINGLE_PROCESS__
  template< class TaskFactoryPOAType, class TaskType >
#else
  template< class TaskFactoryPOAType, class TaskType, class TaskImplType, class TaskFactoryType >
#endif
  struct TaskFactoryArgsBase_i : virtual TaskFactoryPOAType,
                                 virtual SObjectBase
  {
    typedef typename TaskType::_ptr_type TaskPtrType;
    typedef typename TaskType::_var_type TaskVarType;
    typedef typename corba::SmartPtrDef< TaskVarType >::type TTaskPtr;

    TaskFactoryArgsBase_i( const CORBA::ORB_var& theORB, 
                           const PortableServer::POA_var& thePOA )
      : SObjectBase( theORB, thePOA )
    {
      std::cout << "TaskFactoryArgsBase_i::TaskFactoryArgsBase_i[ " << this << " ]" << std::endl;

      this->m_create_mutex.lock();
      this->m_publish_mutex.lock();
    }
    
    virtual ~TaskFactoryArgsBase_i()
    {
      std::cout << "TaskFactoryArgsBase_i::~TaskFactoryArgsBase_i[ " << this << " ]" << std::endl;
    }
    
    virtual TaskPtrType create( const char* theInvocationShellScript, 
                                const typename TaskType::TArgs& theArgs )
    {
      std::cout << "TaskFactoryArgsBase_i::create[ " << this << " ] - '" << theInvocationShellScript << "'" << std::endl;
    
      this->m_task_args = theArgs;

#ifndef __USE_CORBA_SINGLE_PROCESS__
      if ( system( ( std::string( theInvocationShellScript ) + "&" ).c_str() ) != 0 )
        return TaskType::_nil();

      this->m_publish_mutex.unlock();
      this->m_create_mutex.lock();
#else
      this->m_publish_mutex.unlock();

      parallel::create_task< TaskImplType, TaskType, TaskFactoryType >( this->ORB(), this->POA(), this->_this() );

      this->m_create_mutex.lock();
#endif
      
      return this->latest();
    }

    virtual void publish( TaskPtrType theTask )
    {
      std::cout << "TaskFactoryArgsBase_i::publish[ " << this << " ]" << std::endl;
    
      this->m_publish_mutex.lock();

      theTask->init( this->m_task_args );

      TTaskPtr a_task( TaskType::_duplicate( theTask ) );

      this->m_task_pool.push( a_task );

      this->m_create_mutex.unlock();
    }

    TaskPtrType latest()
    {
      return TaskType::_duplicate( *this->m_task_pool.front() );
    }

  protected:
    typename TaskType::TArgs m_task_args;

    typedef std::queue< TTaskPtr > TTaskPool;
    TTaskPool m_task_pool;

    omni_mutex m_create_mutex;
    omni_mutex m_publish_mutex;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
