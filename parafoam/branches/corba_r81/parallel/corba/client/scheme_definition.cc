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
#include "parallel/corba/idl/TaskFactoryA.hh"
#include "parallel/corba/idl/TaskFactoryB.hh"
#include "parallel/corba/idl/TaskManager.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include <iostream>
using namespace std;


//---------------------------------------------------------------------------
static CORBA::Object_ptr getObjectReference( CORBA::ORB_ptr orb, 
                                             const std::string& theObjectType, 
                                             const std::string& theObjectName );


//---------------------------------------------------------------------------
int main( int argc, char **argv ) 
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    CORBA::Object_var a_task_factory_A_obj = getObjectReference( orb, "TaskFactory", "A" );
    parallel::TaskFactoryA_var a_task_factory_A_ref = parallel::TaskFactoryA::_narrow( a_task_factory_A_obj );
    parallel::TaskA_var a_task_A = a_task_factory_A_ref->create();

    CORBA::Object_var a_task_factory_B_obj = getObjectReference( orb, "TaskFactory", "B" );
    parallel::TaskFactoryB_var a_task_factory_B_ref = parallel::TaskFactoryB::_narrow( a_task_factory_B_obj );
    parallel::TaskB_var a_task_B = a_task_factory_B_ref->create();

    CORBA::Object_var a_task_manager_obj = getObjectReference( orb, "TaskManager", "this" );
    parallel::TaskManager_var a_task_manager_ref = parallel::TaskManager::_narrow( a_task_manager_obj );
    a_task_manager_ref->connect( a_task_A, "A", a_task_B, "B" );
    
    {
      parallel::corba::SmartPtrDef< parallel::TaskA_var >::type a_task_A( a_task_factory_A_ref->create() );
    }

    a_task_manager_ref->run();

    orb->destroy();
  }
  catch( CORBA::TRANSIENT& ) {
    cerr << "Caught system exception TRANSIENT -- unable to contact the "
         << "server." << endl;
  }
  catch( CORBA::SystemException& ex ) {
    cerr << "Caught a CORBA::" << ex._name() << endl;
  }
  catch( CORBA::Exception& ex ) {
    cerr << "Caught CORBA::Exception: " << ex._name() << endl;
  }
  catch( omniORB::fatalException& fe ) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }

  return 0;
}


//---------------------------------------------------------------------------
static CORBA::Object_ptr getObjectReference( CORBA::ORB_ptr orb, 
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
