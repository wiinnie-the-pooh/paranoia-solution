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
#ifndef corba_server_TaskBase_i_hh
#define corba_server_TaskBase_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/idl/TaskBase.hh"

#include "parallel/corba/server/SObjectBase.hh"


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  struct TaskBase_i : virtual POA_parallel::TaskBase, virtual SObjectBase
  {
    TaskBase_i( const CORBA::ORB_var& theORB, 
                const PortableServer::POA_var& thePOA );

    ~TaskBase_i();

    void invoke( TaskManager_ptr theTaskManager );
      
  protected:
    virtual void init();

    virtual CORBA::Boolean step() = 0;

    virtual void destroy();
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
