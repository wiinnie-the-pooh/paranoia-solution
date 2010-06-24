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
#ifndef corba_server_TaskFactoryBase_i_hh
#define corba_server_TaskFactoryBase_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/SObjectBase.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include <omnithread.h>

#include <queue>

#include <iostream>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  template< class TaskFactoryPOAType, class TaskType >
  struct TaskFactoryBase_i : virtual TaskFactoryPOAType,
                             virtual SObjectBase
  {
    typedef typename TaskType::_ptr_type TaskPtrType;
    typedef typename TaskType::_var_type TaskVarType;
    typedef typename corba::SmartPtrDef< TaskVarType >::type TTaskPtr;

    TaskFactoryBase_i( const CORBA::ORB_var& theORB, 
		       const PortableServer::POA_var& thePOA )
      : SObjectBase( theORB, thePOA )
    {
      std::cout << "TaskFactoryBase_i::TaskFactoryBase_i[ " << this << " ]" << std::endl;
    }
    
    virtual ~TaskFactoryBase_i()
    {
      std::cout << "TaskFactoryBase_i::~TaskFactoryBase_i[ " << this << " ]" << std::endl;
    }
    
    virtual void publish( TaskPtrType theTask )
    {
      std::cout << "TaskFactoryBase_i::register[ " << this << " ]" << std::endl;
    
      this->m_register_mutex.unlock();

      TTaskPtr a_task( TaskType::_duplicate( theTask ) );
      this->m_task_pool.push( a_task );
    }

    TaskPtrType latest()
    {
      return TaskType::_duplicate( *this->m_task_pool.front() );
    }

    virtual TaskPtrType create( const char* theInvocationShellScript )
    {
      std::cout << "TaskFactoryBase_i::create[ " << this << " ]" << std::endl;
    
      if ( system( theInvocationShellScript ) != 0 )
	return TaskType::_nil();

      this->m_register_mutex.lock();
      
      return this->latest();
    }

  protected:
    typedef std::queue< TTaskPtr > TTaskPool;
    TTaskPool m_task_pool;

    omni_mutex m_register_mutex;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
