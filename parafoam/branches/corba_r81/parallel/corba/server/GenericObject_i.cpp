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
#include "parallel/corba/server/GenericObject_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  GenericObject_i::GenericObject_i( const CORBA::ORB_var& theORB, 
                                    PortableServer::POA_ptr thePOA )
    : ref_counter( 1 )
    , ORB( theORB )
  {
    cout << "GenericObject_i::GenericObject_i : " << this << endl;

    if( CORBA::is_nil( thePOA ) )
      this->POA = PortableServer::ServantBase::_default_POA();
    else
      this->POA = PortableServer::POA::_duplicate( thePOA );
  }
  

  //---------------------------------------------------------------------------
  GenericObject_i::~GenericObject_i()
  {
    cout << "GenericObject_i::~GenericObject_i() : " << this << endl;
  }


  //---------------------------------------------------------------------------
  PortableServer::POA_ptr GenericObject_i::_default_POA()
  {
    return PortableServer::POA::_duplicate( this->POA );
  }


  //---------------------------------------------------------------------------
  void GenericObject_i::AddRef()
  {
    ++this->ref_counter;
  }


  //---------------------------------------------------------------------------
  void GenericObject_i::Release()
  {
    if ( --this->ref_counter <= 0 ) 
    {
      PortableServer::ObjectId_var anObjectId = this->POA->servant_to_id( this );

      this->POA->deactivate_object( anObjectId.in() );

      this->_remove_ref();
    }
  }


  //---------------------------------------------------------------------------
  char* GenericObject_i::IOR()
  {
    CORBA::Object_var anObject = this->_this();
    CORBA::String_var anIOR = this->ORB->object_to_string( anObject );

    return anIOR._retn();
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
