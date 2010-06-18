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
#include "parallel/corba/server/TaskBase_i.hh"

#include "parallel/corba/idl/TaskManager.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskBase_i::TaskBase_i( const CORBA::ORB_var& theORB, 
                          const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
  {
    cout << "TaskBase_i::TaskBase_i : " << this << endl;
  }


  //---------------------------------------------------------------------------
  TaskBase_i::~TaskBase_i()
  {
    cout << "TaskBase_i::~TaskBase_i() : " << this << endl;
  }


  //---------------------------------------------------------------------------
  void TaskBase_i::invoke( TaskManager_ptr theTaskManager )
  {
    this->init();
    
    while( theTaskManager->is_run() && this->step() );
    
    this->destroy();
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::init()
  {
    cout << "TaskBase_i::init() : " << this << endl;
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::destroy()
  {
    cout << "TaskBase_i::destroy() : " << this << endl;
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
