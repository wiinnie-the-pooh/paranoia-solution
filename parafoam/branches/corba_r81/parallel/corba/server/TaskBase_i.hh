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

#include "parallel/corba/server/TransientObject_i.hh"

#include "parallel/corba/server/PortBase_i.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include "parallel/corba/idl/DataHolderBase.hh"

#include <map>
#include <set>


//---------------------------------------------------------------------------
#ifdef _DEBUG_
#  include <sstream>
#  define TMSG( the_message ) { std::ostringstream a_stream; a_stream << the_message; TaskBase_i::print( this, a_stream.str() ); }
#else
#  define TMSG( the_message )
#endif


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  struct SPortHelperEngine;


  //---------------------------------------------------------------------------
  struct TaskBase_i : virtual POA_parallel::TaskBase, 
                      virtual TransientObject_i
  {
    friend struct SPortHelperEngine;

    TaskBase_i( const CORBA::ORB_var& theORB, 
                const PortableServer::POA_var& thePOA );

    ~TaskBase_i();

    //---------------------------------------------------------------------------
    virtual void invoke( TaskManager_ptr theTaskManager );
    
    //---------------------------------------------------------------------------
    virtual PortBase_ptr get_input_port( const char* theName );

    virtual Ports* get_input_ports();

    virtual CORBA::Boolean connect_input( PortBase_ptr thePort, 
                                          Link_ptr theLink, 
                                          PortBase_ptr theOppositePort );
    
    //---------------------------------------------------------------------------
    virtual PortBase_ptr get_output_port( const char* theName );

    virtual Ports* get_output_ports();

    virtual CORBA::Boolean connect_output( PortBase_ptr thePort, 
                                           Link_ptr theLink, 
                                           PortBase_ptr theOppositePort );

    //---------------------------------------------------------------------------
  protected:
    virtual bool step() = 0;

    virtual void destroy() = 0;

  public:
    typedef PortBase_i* TPortPtr;
    typedef corba::SmartPtrDef< Link_var >::type TLinkPtr;
    typedef corba::SmartPtrDef< DataHolderBase_var >::type TDataHolderPtr;

    typedef std::pair< TLinkPtr, TDataHolderPtr > TDataFactrory;
    typedef std::map< TPortPtr, TDataFactrory > TInputPorts;

    typedef std::set< TLinkPtr > TLinks;
    typedef std::map< TPortPtr, TLinks > TOutputPorts;

    enum EPortType { eInputPort, eOutputPort };

  protected:
    bool define_input_port( const TPortPtr& thePort );
    bool define_output_port( const TPortPtr& thePort );
      
    void init_port( const std::string& thePortName, const DataHolderBase_var& theDataHolder );

    void publish( const std::string& thePortName, const DataHolderBase_var& theDataHolder );

    TDataHolderPtr wait_for( const std::string& thePortName );

    template< class DataHolderType, class DataHolderVarType >
    typename corba::SmartPtrDef< DataHolderVarType >::type 
    retrieve( const std::string& theName )
    {
      typedef typename corba::SmartPtrDef< DataHolderVarType >::type DataHolderPtrType;

      TDataHolderPtr aDataHolderBase = this->wait_for( theName );
      if ( !aDataHolderBase )
        return DataHolderPtrType();
        
      DataHolderVarType aDataHolder( DataHolderType::_narrow( *aDataHolderBase ) );
      if ( CORBA::is_nil( aDataHolder ) )
        return DataHolderPtrType();
      
      aDataHolder->AddRef();

      return DataHolderPtrType( aDataHolder );
    }

    static void print( const TaskBase_i* theTask, const std::string& theMessage );

 protected:
    TInputPorts m_input_ports;
    TOutputPorts m_output_ports;

    long m_step_counter;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
