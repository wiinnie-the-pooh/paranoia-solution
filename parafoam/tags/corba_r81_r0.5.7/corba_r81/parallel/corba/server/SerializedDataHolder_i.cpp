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
#include "parallel/corba/server/SerializedDataHolder_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  SerializedDataHolder_i::SerializedDataHolder_i( const std::string& the_value,
                                                  const CORBA::ORB_var& theORB, 
                                                  const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , m_serialized_data( the_value )
  {
    cout << "SerializedDataHolder_i::SerializedDataHolder_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  SerializedDataHolder_i::~SerializedDataHolder_i()
  {
    cout << "DataHolderBase_i::~DataHolderBase_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  char* SerializedDataHolder_i::value()
  {
    cout << "SerializedDataHolder::value[ " << this << " ]" << endl;
    
    return CORBA::string_dup( this->m_serialized_data.c_str() );
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
