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
#include "parallel/corba/idl/PortBase.hh"
#include "parallel/corba/idl/Link.hh"

#include "parallel/corba/server/TaskFactory_utilities.hpp"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace
{
  //---------------------------------------------------------------------------
  using namespace parallel;


  //---------------------------------------------------------------------------
  PortBase_ptr get_port( const char* theName, const TaskBase_i::TPorts& thePorts )
  {
    TaskBase_i::TPorts::const_iterator anIter = thePorts.begin();
    TaskBase_i::TPorts::const_iterator anEnd = thePorts.end();
    for ( int anId = 0; anIter != anEnd; anIter++, anId++ )
    {
      const TaskBase_i::TPortPtr& a_port = anIter->first;

      CORBA::String_var a_name = a_port->name();
      
      if ( strcmp( a_name.in(), theName ) == 0 )
        return a_port->_this();
    }

    return PortBase::_nil();
  }


  //---------------------------------------------------------------------------
  Ports* get_ports( const TaskBase_i::TPorts& thePorts )
  {
    Ports& a_ports = * new Ports;
    a_ports.length( thePorts.size() );

    TaskBase_i::TPorts::const_iterator anIter = thePorts.begin();
    TaskBase_i::TPorts::const_iterator anEnd = thePorts.end();
    for ( int anId = 0; anIter != anEnd; anIter++, anId++ )
    {
      const TaskBase_i::TPortPtr& a_port = anIter->first;
      
      a_ports[ anId ] = a_port->_this();
    }

    return &a_ports;
  }
    
    
  //---------------------------------------------------------------------------
}    


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskBase_i::TaskBase_i( const CORBA::ORB_var& theORB, 
                          const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
  {
    cout << "TaskBase_i::TaskBase_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  TaskBase_i::~TaskBase_i()
  {
    cout << "TaskBase_i::~TaskBase_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  void TaskBase_i::invoke( TaskManager_ptr theTaskManager )
  {
    this->init();
    
    while( theTaskManager->is_run() && this->step() );
    
    this->destroy();
  }
    
    
  //---------------------------------------------------------------------------
  PortBase_ptr TaskBase_i::get_input_port( const char* theName )
  {
    return ::get_port( theName, this->m_input_ports );
  }
    
    
  //---------------------------------------------------------------------------
  Ports* TaskBase_i::get_input_ports()
  {
    return ::get_ports( this->m_input_ports );
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::connect_input( PortBase_ptr thePort, 
                                  Link_ptr theLink, 
                                  PortBase_ptr theOppositePort )
  {
    thePort->is_compatible( theOppositePort );
      
    PortBase_i* aPort = get_servant< PortBase_i* >( thePort, this->POA );

    TLinkPtr aLink( Link::_duplicate( theLink ) );

    this->m_input_ports[ aPort ] = aLink;
  }
    
    
  //---------------------------------------------------------------------------
  PortBase_ptr TaskBase_i::get_output_port( const char* theName )
  {
    return ::get_port( theName, this->m_output_ports );
  }
    
    
  //---------------------------------------------------------------------------
  Ports* TaskBase_i::get_output_ports()
  {
    return ::get_ports( this->m_output_ports );
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::connect_output( PortBase_ptr thePort, 
				   Link_ptr theLink, 
				   PortBase_ptr theOppositePort )
  {
    thePort->is_compatible( theOppositePort );
      
    PortBase_i* aPort = get_servant< PortBase_i* >( thePort, this->POA );

    TLinkPtr aLink( Link::_duplicate( theLink ) );

    this->m_output_ports[ aPort ] = aLink;
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::init()
  {
    cout << "TaskBase_i::init[ " << this << " ]" << endl;
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::destroy()
  {
    cout << "TaskBase_i::destroy[ " << this << " ]" << endl;
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
