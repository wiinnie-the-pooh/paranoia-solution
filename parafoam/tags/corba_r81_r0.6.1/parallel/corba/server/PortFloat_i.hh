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
#ifndef corba_server_PortFloat_i_hh
#define corba_server_PortFloat_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/idl/PortFloat.hh"

#include "parallel/corba/server/PortBase_i.hh"

#include "parallel/corba/server/DataHolderBase_i.hh"

#include "parallel/corba/server/SSimpleValueHelper.hh"


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  struct DataHolderFloat_i : virtual POA_parallel::DataHolderFloat, 
                            virtual DataHolderBase_i
  {
    DataHolderFloat_i( CORBA::Long theValue,
                       const CORBA::ORB_var& theORB, 
                       const PortableServer::POA_var& thePOA );

    static DataHolderFloat_ptr create( CORBA::Long theValue,
                                       const CORBA::ORB_var& theORB, 
                                       const PortableServer::POA_var& thePOA );
    
    ~DataHolderFloat_i();

    CORBA::Double value();

    typedef parallel::DataHolderFloat Interface;

  protected:
    CORBA::Double m_value;
  };


  //---------------------------------------------------------------------------
  struct PortFloat_i : virtual POA_parallel::PortFloat, 
                      virtual PortBase_i
  {
    PortFloat_i( const std::string& theName,
                const CORBA::ORB_var& theORB, 
                const PortableServer::POA_var& thePOA );

    ~PortFloat_i();

    CORBA::Boolean is_compatible( PortBase_ptr theArg );

    typedef DataHolderFloat_i TDataHolder_i;
    typedef SSimpleValueHelper< CORBA::Double > TValueHelper;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
