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
#include <TaskFactory.hh>

#include <iostream>
using namespace std;


//---------------------------------------------------------------------------
static CORBA::Object_ptr getObjectReference( CORBA::ORB_ptr orb );


//---------------------------------------------------------------------------
int main( int argc, char **argv ) 
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    CORBA::Object_var obj = getObjectReference( orb );

    TaskFactory_var a_task_factory_ref = TaskFactory::_narrow( obj );

    Task_var a_task = a_task_factory_ref->create();

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
static CORBA::Object_ptr getObjectReference( CORBA::ORB_ptr orb )
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

  name[ 0 ].id   = (const char*) "TaskFactory";
  name[ 0 ].kind = (const char*) "paraFoam";

  name[ 1 ].id   = (const char*) "XXX";
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
