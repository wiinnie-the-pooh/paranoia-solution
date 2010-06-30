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
#include "parallel/corba/server/PortInt_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  PortInt_i::PortInt_i( const std::string& theName,
			const CORBA::ORB_var& theORB, 
			const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , PortBase_i( theName, theORB, thePOA )
  {
    cout << "PortInt_i::PortInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  PortInt_i::~PortInt_i()
  {
    cout << "PortInt_i::~PortInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean PortInt_i::is_compatible( PortBase_ptr theArg )
  {
    cout << "PortInt_i::is_compatible[ " << this << " ]" << endl;

    PortInt_var aPort = parallel::PortInt::_narrow( theArg );
    
    return ! CORBA::is_nil( aPort );
  }


  //---------------------------------------------------------------------------
  DataHolderInt_i::DataHolderInt_i( CORBA::Long theValue,
				    const CORBA::ORB_var& theORB, 
				    const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , DataHolderBase_i( theORB, thePOA )
    , m_value( theValue )
  {
    cout << "DataHolderInt_i::DataHolderInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  DataHolderInt_ptr DataHolderInt_i::create( CORBA::Long theValue,
					     const CORBA::ORB_var& theORB, 
					     const PortableServer::POA_var& thePOA )
  {
    DataHolderInt_i* an_item( new DataHolderInt_i( theValue, theORB, thePOA ) );
    
    return an_item->_this();
  }


  //---------------------------------------------------------------------------
  DataHolderInt_i::~DataHolderInt_i()
  {
    cout << "DataHolderInt_i::~DataHolderInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Long DataHolderInt_i::value()
  {
    cout << "DataHolderInt_i::value[ " << this << " ]" << endl;

    return this->m_value;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
