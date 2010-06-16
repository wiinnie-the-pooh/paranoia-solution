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
#include <parallel/corba/idl/TaskFactoryA.hh>

#include <parallel/corba/server/TaskFactory_utilities.hpp>

#include <iostream>
using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  struct TaskFactoryA_i 
  {
    inline TaskFactoryA_i() {}
    
    inline ~TaskFactoryA_i() {}
    
    TaskA_ptr create()
    {
      cout << "TaskFactoryA_i::create()" << endl;
      
      return TaskA::_nil();
    }
  };
}


//---------------------------------------------------------------------------
int main( int argc, char** argv )
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    CORBA::Object_var poa_obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( poa_obj );

    using namespace parallel;
    TaskFactoryA_i* a_task_factory_impl = new TaskFactoryA_i();
    POA_parallel::TaskFactoryA_tie< TaskFactoryA_i > a_task_factory_tie( a_task_factory_impl );

    PortableServer::ObjectId_var a_task_factory_id = poa->activate_object( &a_task_factory_tie );

    // Obtain a reference to the object, and register it in the naming service.
    CORBA::Object_var a_task_factory_obj = a_task_factory_tie._this();
    if( !bindObjectToName( orb, a_task_factory_obj ) )
      return 1;

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
  }
  catch( CORBA::SystemException& ex ) {
    cerr << "Caught CORBA::" << ex._name() << endl;
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
