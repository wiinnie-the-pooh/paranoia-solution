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

#include "parallel/corba/common/corba_utilities.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace
{
  //---------------------------------------------------------------------------
  using namespace parallel;

  typedef std::map< std::string, TaskBase_i::TPortPtr > TName2Port;


  //---------------------------------------------------------------------------
  void _get_input_ports_( const TaskBase_i::TInputPorts& theInputPorts,
                          TName2Port& theName2Port )
  {
    TaskBase_i::TInputPorts::const_iterator anIter = theInputPorts.begin();
    TaskBase_i::TInputPorts::const_iterator anEnd = theInputPorts.end();
    for ( ; anIter != anEnd; anIter++ )
    {
      const TaskBase_i::TPortPtr& a_port = anIter->first;
      CORBA::String_var a_name = a_port->name();
      theName2Port[ a_name.in() ] = a_port;
    }
  }
    
    
  //---------------------------------------------------------------------------
  void _get_output_ports_( const TaskBase_i::TOutputPorts& theOutputPorts,
                           TName2Port& theName2Port )
  {
    TaskBase_i::TOutputPorts::const_iterator anIter = theOutputPorts.begin();
    TaskBase_i::TOutputPorts::const_iterator anEnd = theOutputPorts.end();
    for ( ; anIter != anEnd; anIter++ )
    {
      const TaskBase_i::TPortPtr& a_port = anIter->first;
      CORBA::String_var a_name = a_port->name();
      theName2Port[ a_name.in() ] = a_port;
    }
  }
    
    
  //---------------------------------------------------------------------------
  PortBase_ptr get_port( const char* theName, const TName2Port& theName2Port )
  {
    TName2Port::const_iterator anIter = theName2Port.find( theName );

    if ( anIter == theName2Port.end() )
      return PortBase::_nil();

    const TaskBase_i::TPortPtr& a_port = anIter->second;

    return a_port->_this();
  }


  //---------------------------------------------------------------------------
  Ports* get_ports( const TName2Port& theName2Port )
  {
    Ports& a_ports = * new Ports;
    a_ports.length( theName2Port.size() );

    TName2Port::const_iterator anIter = theName2Port.begin();
    TName2Port::const_iterator anEnd = theName2Port.end();
    for ( int anId = 0; anIter != anEnd; anIter++, anId++ )
    {
      const TaskBase_i::TPortPtr& a_port = anIter->second;
      
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
    while( theTaskManager->is_run() && this->step() );
    
    this->destroy();
  }
    
    
  //---------------------------------------------------------------------------
  PortBase_ptr TaskBase_i::get_input_port( const char* theName )
  {
    ::TName2Port aName2Port;
    ::_get_input_ports_( this->m_input_ports, aName2Port );

    return ::get_port( theName, aName2Port );
  }
    
    
  //---------------------------------------------------------------------------
  Ports* TaskBase_i::get_input_ports()
  {
    ::TName2Port aName2Port;
    ::_get_input_ports_( this->m_input_ports, aName2Port );

    return ::get_ports( aName2Port );
  }
    
    
  //---------------------------------------------------------------------------
  CORBA::Boolean TaskBase_i::connect_input( PortBase_ptr thePort, 
                                            Link_ptr theLink, 
                                            PortBase_ptr theOppositePort )
  {
    if ( !thePort->is_compatible( theOppositePort ) )
      return false;
      
    PortBase_i* aPort = get_servant< PortBase_i* >( thePort, this->POA() );
    if ( !aPort )
      return false;

    if ( this->m_input_ports.find( aPort ) == this->m_input_ports.end() )
      return false;

    TaskBase_i::TLinkPtr aLink( Link::_duplicate( theLink ) );
    TDataFactrory& a_data_factory = this->m_input_ports[ aPort ];
    a_data_factory.first = aLink;

    return true;
  }
    
    
  //---------------------------------------------------------------------------
  PortBase_ptr TaskBase_i::get_output_port( const char* theName )
  {
    ::TName2Port aName2Port;
    ::_get_output_ports_( this->m_output_ports, aName2Port );

    return ::get_port( theName, aName2Port );
  }
    
    
  //---------------------------------------------------------------------------
  Ports* TaskBase_i::get_output_ports()
  {
    ::TName2Port aName2Port;
    ::_get_output_ports_( this->m_output_ports, aName2Port );

    return ::get_ports( aName2Port );
  }
    
    
  //---------------------------------------------------------------------------
  CORBA::Boolean TaskBase_i::connect_output( PortBase_ptr thePort, 
                                             Link_ptr theLink, 
                                             PortBase_ptr theOppositePort )
  {
    if ( !thePort->is_compatible( theOppositePort ) )
      return false;
      
    PortBase_i* aPort = get_servant< PortBase_i* >( thePort, this->POA() );
    if ( !aPort )
      return false;

    if ( this->m_output_ports.find( aPort ) == this->m_output_ports.end() )
      return false;

    TaskBase_i::TLinkPtr aLink( Link::_duplicate( theLink ) );
    TLinks& a_links = this->m_output_ports[ aPort ];
    a_links.insert( aLink );

    return true;
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
  bool TaskBase_i::define_input_port( const TPortPtr& thePort )
  {
    if ( this->m_input_ports.find( thePort ) != this->m_input_ports.end() )
      return false;
      
    this->m_input_ports[ thePort ];
      
    return true;
  }

    
  //---------------------------------------------------------------------------
  bool TaskBase_i::define_output_port( const TPortPtr& thePort )
  {
    if ( this->m_output_ports.find( thePort ) != this->m_output_ports.end() )
      return false;
      
    this->m_output_ports[ thePort ];
      
    return true;
  }

    
  //---------------------------------------------------------------------------
  void TaskBase_i::init_port( const std::string& thePortName, const DataHolderBase_var& theDataHolder )
  {
    ::TName2Port aName2Port;
    ::_get_input_ports_( this->m_input_ports, aName2Port );
    
    TName2Port::const_iterator anIter = aName2Port.find( thePortName );
    if ( anIter == aName2Port.end() )
      return;

    const TPortPtr& aPort = anIter->second;
    TDataFactrory& aDataFactrory = this->m_input_ports[ aPort ];
    if ( const TLinkPtr& aLink = aDataFactrory.first )
    {
      aLink->publish( theDataHolder );
    }
    else
    {
      // In case if there is no link connected to this input port
      // this "theDataHolder" value will be used by default during 
      // the simulation time
      TDataHolderPtr& aDataHolderPtr = aDataFactrory.second;
      aDataHolderPtr = TDataHolderPtr( theDataHolder );
    }
  }
    
    
  //---------------------------------------------------------------------------
  void TaskBase_i::publish( const std::string& thePortName, const DataHolderBase_var& theDataHolder )
  {
    ::TName2Port aName2Port;
    ::_get_output_ports_( this->m_output_ports, aName2Port );
    
    TName2Port::const_iterator anIter = aName2Port.find( thePortName );
    if ( anIter == aName2Port.end() )
      return;

    const TPortPtr& aPort = anIter->second;
    const TLinks& a_links = this->m_output_ports[ aPort ];
    {
      TLinks::const_iterator anIter = a_links.begin();
      TLinks::const_iterator anEnd = a_links.end();
      for ( ; anIter != anEnd; anIter++ ) {
        const TLinkPtr& a_link = *anIter;
        a_link->publish( theDataHolder );
      }
    }
  }
      
      
    
    
  //---------------------------------------------------------------------------
  TaskBase_i::TDataHolderPtr TaskBase_i::wait_for( const std::string& thePortName )
  {
    ::TName2Port aName2Port;
    ::_get_input_ports_( this->m_input_ports, aName2Port );
    
    TName2Port::const_iterator anIter = aName2Port.find( thePortName );
    if ( anIter == aName2Port.end() )
      return TDataHolderPtr();

    const TPortPtr& aPort = anIter->second;
    TDataFactrory& aDataFactrory = this->m_input_ports[ aPort ];
    if ( const TLinkPtr& aLink = aDataFactrory.first )
    {
      return TDataHolderPtr( aLink->retrieve() );
    }
    else
    {
      const TDataHolderPtr& aDataHolder = aDataFactrory.second;
      return aDataHolder;
    }
  }
      
      
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
