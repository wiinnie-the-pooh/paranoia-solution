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
#include "parallel/corba/server/PortFloat_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  PortFloat_i::PortFloat_i( const std::string& theName,
                            const CORBA::ORB_var& theORB, 
                            const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , PortBase_i( theName, theORB, thePOA )
  {
    cout << "PortFloat_i::PortFloat_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  PortFloat_i::~PortFloat_i()
  {
    cout << "PortFloat_i::~PortFloat_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean PortFloat_i::is_compatible( PortBase_ptr theArg )
  {
    cout << "PortFloat_i::is_compatible[ " << this << " ]" << endl;

    PortFloat_var aPort = parallel::PortFloat::_narrow( theArg );
    
    return ! CORBA::is_nil( aPort );
  }


  //---------------------------------------------------------------------------
  DataHolderFloat_i::DataHolderFloat_i( CORBA::Long theValue,
                                        const CORBA::ORB_var& theORB, 
                                        const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , DataHolderBase_i( theORB, thePOA )
    , m_value( theValue )
  {
    cout << "DataHolderFloat_i::DataHolderFloat_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  DataHolderFloat_ptr DataHolderFloat_i::create( CORBA::Long theValue,
                                                 const CORBA::ORB_var& theORB, 
                                                 const PortableServer::POA_var& thePOA )
  {
    DataHolderFloat_i* an_item( new DataHolderFloat_i( theValue, theORB, thePOA ) );
    
    return an_item->_this();
  }


  //---------------------------------------------------------------------------
  DataHolderFloat_i::~DataHolderFloat_i()
  {
    cout << "DataHolderFloat_i::~DataHolderFloat_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Double DataHolderFloat_i::value()
  {
    cout << "DataHolderFloat_i::value[ " << this << " ]" << endl;

    return this->m_value;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
