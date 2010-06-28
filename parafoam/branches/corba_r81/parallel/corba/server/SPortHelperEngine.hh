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
#ifndef corba_server_SPortHelperEngine_hh
#define corba_server_SPortHelperEngine_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/PortBase_i.hh"

#include "parallel/corba/server/TaskBase_i.hh"

#include "parallel/corba/server/DataHolderBase_i.hh"

#include "parallel/corba/CORBASmartPtr.hh"


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  struct SPortHelperEngine
  {
  protected:
    typedef PortBase_i* TPortPtr;
    typedef TaskBase_i* TTaskPtr;
    typedef DataHolderBase_var TDataHolderBaseVar;

    SPortHelperEngine( const TPortPtr& thePort, 
		       TaskBase_i::EPortType thePortType, 
		       const TTaskPtr& theTask );

    const std::string& c_name();
      
    TTaskPtr task();

    const CORBA::ORB_var& ORB();

    const PortableServer::POA_var& POA();

    void __init__( const TTaskPtr& theTask,
		   const std::string& theName, 
		   const TDataHolderBaseVar& theDataHolder );

    void __publish__( const TTaskPtr& theTask,
		      const std::string& theName, 
		      const TDataHolderBaseVar& theDataHolder );

    template< class DataHolderType, class DataHolderVarType >
    typename corba::SmartPtrDef< DataHolderVarType >::type 
    __retrieve__( const TTaskPtr& theTask,
		  const std::string& theName )
    {
      return theTask->template retrieve< DataHolderType, DataHolderVarType >( theName );
    }

  protected:
    TPortPtr m_port;
    TTaskPtr m_task;
  };
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
