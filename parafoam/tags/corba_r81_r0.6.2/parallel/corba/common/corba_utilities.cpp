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
#include "parallel/corba/common/corba_utilities.hh"

#include <string>
#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  PortableServer::ServantBase_var _get_servant( CORBA::Object_ptr theObject,
                                                const PortableServer::POA_var& thePOA )
  {
    if ( CORBA::is_nil( theObject ) )  
      return NULL;

    try {
      return thePOA->reference_to_servant( theObject );
    } catch (...) {
      return NULL;
    }

    return NULL;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean bindObjectToName( CORBA::ORB_ptr orb,
                                   CORBA::Object_ptr objref, 
                                   const std::string& theObjectType, 
                                   const std::string& theObjectName )
  {
    CosNaming::NamingContext_var rootContext;
    try {
      // Obtain a reference to the root context of the Name service:
      CORBA::Object_var obj;
      obj = orb->resolve_initial_references( "NameService" );
      rootContext = CosNaming::NamingContext::_narrow( obj );
      if( CORBA::is_nil( rootContext ) ) {
        cerr << "Failed to narrow the root naming context." << endl;
        return 0;
      }
    }
    catch ( CORBA::NO_RESOURCES& ) {
      cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
           << "with the location" << endl
           << "of the naming service." << endl;
      return 0;
    }
    catch ( CORBA::ORB::InvalidName& ) {
      // This should not happen!
      cerr << "Service required is invalid [does not exist]." << endl;
      return 0;
    }
  
    try {
      CosNaming::Name contextName;
      contextName.length( 1 );
      contextName[ 0 ].id   = (const char*) theObjectType.c_str();
      contextName[ 0 ].kind = (const char*) "parallel";
  
      CosNaming::NamingContext_var paraFoamContext;
      try {
        paraFoamContext = rootContext->bind_new_context( contextName );
      }
      catch( CosNaming::NamingContext::AlreadyBound& ex ) {
        CORBA::Object_var obj;
        obj = rootContext->resolve( contextName );
        paraFoamContext = CosNaming::NamingContext::_narrow( obj );
        if( CORBA::is_nil( paraFoamContext ) ) {
          cerr << "Failed to narrow naming context." << endl;
          return 0;
        }
      }
  
      CosNaming::Name objectName;
      objectName.length( 1 );
      objectName[ 0 ].id   = (const char*) theObjectName.c_str();
      objectName[ 0 ].kind = (const char*) "object";
  
      try {
        paraFoamContext->bind( objectName, objref );
      }
      catch( CosNaming::NamingContext::AlreadyBound& ex ) {
        paraFoamContext->rebind( objectName, objref );
      }
    }
    catch( CORBA::TRANSIENT& ex ) {
      cerr << "Caught system exception TRANSIENT -- unable to contact the "
           << "naming service." << endl
           << "Make sure the naming server is running and that omniORB is "
           << "configured correctly." << endl;
  
      return 0;
    }
    catch( CORBA::SystemException& ex ) {
      cerr << "Caught a CORBA::" << ex._name()
           << " while using the naming service." << endl;
      return 0;
    }
  
    return 1;
  }


  //---------------------------------------------------------------------------
  CORBA::Object_ptr getObjectReference( CORBA::ORB_ptr orb, 
                                        const std::string& theObjectType, 
                                        const std::string& theObjectName )
  {
    CosNaming::NamingContext_var rootContext;
    
    try {
      // Obtain a reference to the root context of the Name service:
      CORBA::Object_var obj;
      obj = orb->resolve_initial_references( "NameService" );
      rootContext = CosNaming::NamingContext::_narrow( obj );
      if( CORBA::is_nil( rootContext ) ) {
        cerr << "Failed to narrow the root naming context." << endl;
        return CORBA::Object::_nil();
      }
    }
    catch ( CORBA::NO_RESOURCES& ) {
      cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
           << "with the location" << endl
           << "of the naming service." << endl;
      
      return 0;
    }
    catch( CORBA::ORB::InvalidName& ex ) {
      // This should not happen!
      cerr << "Service required is invalid [does not exist]." << endl;
      
      return CORBA::Object::_nil();
    }
    
    // Create a name object, containing the name test/context:
    CosNaming::Name name;
    name.length( 2 );
    
    name[ 0 ].id   = (const char*) theObjectType.c_str();
    name[ 0 ].kind = (const char*) "parallel";
    
    name[ 1 ].id   = (const char*) theObjectName.c_str();
    name[ 1 ].kind = (const char*) "object";
    
    try {
      return rootContext->resolve( name );
    }
    catch( CosNaming::NamingContext::NotFound& ex ) {
      cerr << "Context not found." << endl;
    }
    catch( CORBA::TRANSIENT& ex ) {
      cerr << "Caught system exception TRANSIENT -- unable to contact the "
           << "naming service." << endl
           << "Make sure the naming server is running and that omniORB is "
           << "configured correctly." << endl;
    }
    catch( CORBA::SystemException& ex ) {
      cerr << "Caught a CORBA::" << ex._name()
           << " while using the naming service." << endl;
      
      return 0;
    }
    
    return CORBA::Object::_nil();
  }
  
  
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
