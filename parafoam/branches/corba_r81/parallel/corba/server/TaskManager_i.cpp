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
#include <parallel/corba/server/TaskManager_i.hh>

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskManager_i::TaskManager_i( const CORBA::ORB_var& theORB, 
                                const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
  {
    cout << "TaskManager_i::TaskManager_i : " << this << endl;
  }


  //---------------------------------------------------------------------------
  TaskManager_i::~TaskManager_i()
  {
    cout << "TaskManager_i::~TaskManager_i() : " << this << endl;
  }


  //---------------------------------------------------------------------------
  void TaskManager_i::connect( TaskBase_ptr theSourceTask, 
                               const char* theOutputPortName,
                               TaskBase_ptr theTargetTask, 
                               const char* theInputPortName )
  {
    std::cout << "TaskManager_i::connect : " << this << " - '" << theOutputPortName << "'; '" << theInputPortName << "'" << std::endl;

    this->register_task( theSourceTask );
    this->register_task( theTargetTask );
  }
    
    
  //---------------------------------------------------------------------------
  void TaskManager_i::register_task( TaskBase_ptr theTask )
  {
    std::cout << "TaskManager_i::add : " << this << std::endl;

    CORBA::String_var anIOR = this->ORB->object_to_string( theTask );

    this->tasks.insert( anIOR.in() );
  }
    
    
  //---------------------------------------------------------------------------
  CORBA::Boolean TaskManager_i::is_run()
  {
    cout << "TaskManager_i::is_run() : " << this << endl;
    
    return false;
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
