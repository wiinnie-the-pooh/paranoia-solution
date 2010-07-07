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
#ifndef corba_server_TaskManager_i_hh
#define corba_server_TaskManager_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/idl/TaskManager.hh"

#include "parallel/corba/server/SObjectBase.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include <map>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  struct TaskManager_i : virtual POA_parallel::TaskManager, virtual SObjectBase
  {
    TaskManager_i( const CORBA::ORB_var& theORB, 
                   const PortableServer::POA_var& thePOA );
    
    ~TaskManager_i();
    
    virtual CORBA::Boolean connect( TaskBase_ptr theSourceTask, 
                                    const char* theOutputPortName,
                                    TaskBase_ptr theTargetTask, 
                                    const char* theInputPortName );
    
    virtual void register_task( TaskBase_ptr theTask );

    virtual void start();

    virtual void wait();

    virtual void stop();

    virtual void pause();

    virtual void resume();

    virtual CORBA::Boolean is_run();

    typedef corba::SmartPtrDef< TaskBase_var >::type TTaskBasePtr;

  protected:
    typedef std::map< TTaskBasePtr, omni_thread* > TTaskSet;
    TTaskSet tasks;

    omni_mutex m_pause_mutex;

    bool m_is_run;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
