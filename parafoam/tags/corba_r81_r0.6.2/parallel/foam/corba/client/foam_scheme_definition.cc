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
#include "parallel/foam/corba/idl/Foam_TimeSourceTaskFactory.hh"
#include "parallel/corba/idl/TaskManager.hh"

#include "parallel/corba/common/corba_utilities.hh"

#include "parallel/corba/CORBASmartPtr.hh"


//---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__

#include "parallel/corba/server/TaskManager_i.hh"
#include "parallel/foam/corba/server/Foam_TimeSourceTaskFactory_i.hh"

#include "parallel/corba/common/FactoryLauncher.hh"
#include "parallel/corba/common/TaskLauncher.hh"

#endif


//---------------------------------------------------------------------------
#include <iostream>
using namespace std;


//---------------------------------------------------------------------------
int main( int argc, char **argv ) 
{
  try {
    using namespace parallel;
    using namespace parallel::foam;

    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );
  
    //---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__
    CORBA::Object_var poa_obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( poa_obj );
    
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();
    
    TaskManager_var a_task_manager = create_factory< TaskManager_i, TaskManager >( orb, poa, "TaskManager", "this" );

    TimeSourceTaskFactory_var a_time_source_task_factory = 
      create_factory< TimeSourceTaskFactory_i, TimeSourceTaskFactory >( orb, poa, "TaskFactory", "Foam_TimeSource" );

#else
    TaskManager_var a_task_manager = TaskManager::_narrow( getObjectReference( orb, "TaskManager", "this" ) );

    TimeSourceTaskFactory_var a_time_source_task_factory = 
      TimeSourceTaskFactory::_narrow( getObjectReference( orb, "TaskFactory", "Foam_TimeSource" ) );

#endif

    //---------------------------------------------------------------------------
    TimeSourceTask_var a_time_source_task = a_time_source_task_factory->create( "Foam_TimeSourceTask_launcher" );
    a_time_source_task->setTime( dimensionedScalar( 0.000 ), 0 );
    a_time_source_task->setDeltaT( dimensionedScalar( 0.005 ) );
    a_time_source_task->setEndTime( dimensionedScalar( 0.033 ) );
    a_time_source_task->setWriteInterval( 1 );

    a_task_manager->register_task( a_time_source_task );
    
    a_time_source_task->Release();

    a_task_manager->start();

    a_task_manager->wait();

    orb->destroy();

    cout << "\nEnd\n";

    return 0;
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

  return -1;
}


//---------------------------------------------------------------------------
