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
#include "parallel/corba/server/PortBool_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  PortBool_i::PortBool_i( const std::string& theName,
                          const CORBA::ORB_var& theORB, 
                          const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , PortBase_i( theName, theORB, thePOA )
  {
    cout << "PortBool_i::PortBool_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  PortBool_i::~PortBool_i()
  {
    cout << "PortBool_i::~PortBool_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean PortBool_i::is_compatible( PortBase_ptr theArg )
  {
    cout << "PortBool_i::is_compatible[ " << this << " ]" << endl;

    PortBool_var aPort = parallel::PortBool::_narrow( theArg );
    
    return ! CORBA::is_nil( aPort );
  }


  //---------------------------------------------------------------------------
  DataHolderBool_i::DataHolderBool_i( bool theValue,
                                      const CORBA::ORB_var& theORB, 
                                      const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , DataHolderBase_i( theORB, thePOA )
    , m_value( theValue )
  {
    cout << "DataHolderBool_i::DataHolderBool_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  DataHolderBool_ptr DataHolderBool_i::create( bool theValue,
                                               const CORBA::ORB_var& theORB, 
                                               const PortableServer::POA_var& thePOA )
  {
    DataHolderBool_i* an_item( new DataHolderBool_i( theValue, theORB, thePOA ) );
    
    return an_item->_this();
  }


  //---------------------------------------------------------------------------
  DataHolderBool_i::~DataHolderBool_i()
  {
    cout << "DataHolderBool_i::~DataHolderBool_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean DataHolderBool_i::value()
  {
    cout << "DataHolderBool_i::value[ " << this << " ]" << endl;

    return this->m_value;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
