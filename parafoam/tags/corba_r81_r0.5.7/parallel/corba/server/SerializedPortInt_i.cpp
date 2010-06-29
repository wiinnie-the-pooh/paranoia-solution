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
#include "parallel/corba/server/SerializedPortInt_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  SerializedPortInt_i::SerializedPortInt_i( const std::string& theName,
                                            const CORBA::ORB_var& theORB, 
                                            const PortableServer::POA_var& thePOA )
    : SObjectBase( theORB, thePOA )
    , PortBase_i( theName, theORB, thePOA )
    , SerializedPort_i( theName, theORB, thePOA )
  {
    cout << "SerializedPortInt_i::SerializedPortInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  SerializedPortInt_i::~SerializedPortInt_i()
  {
    cout << "SerializedPortInt_i::~SerializedPortInt_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean SerializedPortInt_i::is_compatible( PortBase_ptr theArg )
  {
    cout << "SerializedPortInt_i::is_compatible[ " << this << " ]" << endl;

    SerializedPortInt_var aPort = parallel::SerializedPortInt::_narrow( theArg );
    
    return ! CORBA::is_nil( aPort );
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
