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
#include "parallel/corba/server/TaskFactoryB_i.hh"

#include "parallel/corba/server/TaskBase_i.hh"

#include "parallel/corba/server/PortBool_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  struct TaskB_i : virtual POA_parallel::TaskB, virtual TaskBase_i
  {
    TaskB_i( const CORBA::ORB_var& theORB, 
             const PortableServer::POA_var& thePOA );

    ~TaskB_i();

  protected:
    void init();

    CORBA::Boolean step();
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskB_i::TaskB_i( const CORBA::ORB_var& theORB, 
                    const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , TaskBase_i( theORB, thePOA )
  {
    cout << "TaskB_i::TaskB_i[ " << this << " ]" << endl;

    this->define_input_port( new PortBool_i( "y", this->ORB, this->POA ) );
  }


  //---------------------------------------------------------------------------
  TaskB_i::~TaskB_i()
  {
    cout << "TaskB_i::~TaskB_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  void TaskB_i::init()
  {
    this->init_port( "y", DataHolderBool_i::create( true, this->ORB, this->POA ) );
    
    cout << "TaskB_i::init[ " << this << " ]" << endl;
  }
    
    
  //---------------------------------------------------------------------------
  CORBA::Boolean TaskB_i::step()
  {
    typedef corba::SmartPtrDef< DataHolderBool_var >::type DataHolderBoolPtr;
    DataHolderBoolPtr aDataHolder = this->retrieve< DataHolderBool, DataHolderBool_var >( "y" );

    cout << "TaskB_i::step[ " << this << " ]" << endl;
    
    return aDataHolder->value();
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskFactoryB_i::TaskFactoryB_i( const CORBA::ORB_var& theORB, 
                                  const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , TaskFactoryBase_i< POA_parallel::TaskFactoryB, TaskB >( theORB, thePOA )
  {
    cout << "TaskFactoryB_i::TaskFactoryB_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  TaskFactoryB_i::~TaskFactoryB_i()
  {
    cout << "TaskFactoryB_i::~TaskFactoryB_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  TaskB_ptr TaskFactoryB_i::create( const char* theHostName )
  {
    cout << "TaskFactoryB_i::create[ " << this << " ]" << endl;
    
    this->m_register_mutex.lock();
      
    TaskB_i* a_task = new TaskB_i( this->ORB, this->POA );

    return a_task->_this();
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
