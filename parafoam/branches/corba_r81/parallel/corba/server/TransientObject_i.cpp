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
#include "parallel/corba/server/TransientObject_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TransientObject_i::TransientObject_i( const CORBA::ORB_var& theORB, 
                                        PortableServer::POA_ptr thePOA )
    : ref_counter( 1 )
    , m_ORB( theORB )
  {
    cout << "TransientObject_i::TransientObject_i[ " << this << " ]" << endl;

    if( CORBA::is_nil( thePOA ) )
      this->m_POA = PortableServer::ServantBase::_default_POA();
    else
      this->m_POA = PortableServer::POA::_duplicate( thePOA );
  }
  

  //---------------------------------------------------------------------------
  TransientObject_i::~TransientObject_i()
  {
    cout << "TransientObject_i::~TransientObject_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  PortableServer::POA_ptr TransientObject_i::_default_POA()
  {
    return PortableServer::POA::_duplicate( this->m_POA );
  }


  //---------------------------------------------------------------------------
  void TransientObject_i::AddRef()
  {
    //cout << "TransientObject_i::AddRef[ " << this << " ] : " << this->ref_counter << endl;

    ++this->ref_counter;
  }


  //---------------------------------------------------------------------------
  void TransientObject_i::Release()
  {
    //cout << "TransientObject_i::Release[ " << this << " ] : " << this->ref_counter << endl;

    if ( --this->ref_counter <= 0 ) 
    {
      PortableServer::ObjectId_var anObjectId = this->m_POA->servant_to_id( this );

      this->m_POA->deactivate_object( anObjectId.in() );

      this->_remove_ref();
    }
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean TransientObject_i::equal( TransientObject_ptr theArg )
  {
    if ( CORBA::is_nil( theArg ) )
      return false;

    CORBA::String_var anArg = theArg->IOR();
    CORBA::String_var aSelf = this->IOR();

    return strcmp( aSelf.in(), anArg.in() ) == 0;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean TransientObject_i::less( TransientObject_ptr theArg )
  {
    if ( CORBA::is_nil( theArg ) )
      return false;

    CORBA::String_var anArg = theArg->IOR();
    CORBA::String_var aSelf = this->IOR();

    return strcmp( aSelf.in(), anArg.in() ) < 0;
  }


  //---------------------------------------------------------------------------
  char* TransientObject_i::IOR()
  {
    CORBA::Object_var anObject = this->_this();

    CORBA::String_var anIOR = this->m_ORB->object_to_string( anObject );

    return anIOR._retn();
  }


  //---------------------------------------------------------------------------
  const CORBA::ORB_var& TransientObject_i::ORB()
  {
    return this->m_ORB;
  }


  //---------------------------------------------------------------------------
  const PortableServer::POA_var& TransientObject_i::POA()
  {
    return this->m_POA;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
