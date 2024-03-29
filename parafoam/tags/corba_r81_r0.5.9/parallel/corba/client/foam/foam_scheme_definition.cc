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
#include "foam/Foam_TimeSourceTaskFactory.hh"
//#include "foam/Foam_SolverTaskBase.hh"
#include "TaskManager.hh"

#include "parallel/corba/common/corba_utilities.hh"

#include "parallel/corba/CORBASmartPtr.hh"


//---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__

#include "parallel/corba/server/TaskManager_i.hh"
#include "parallel/corba/server/foam/Foam_TimeSourceTaskFactory_i.hh"
//#include "parallel/corba/server/foam/Foam_SolverTaskBase_i.hh"

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
    
    TimeSourceTaskFactory_var a_time_source_task_factory = create_factory< TimeSourceTaskFactory_i, TimeSourceTaskFactory >( orb, poa, "TaskFactory", "Foam_TimeSource" );
    //SolverTaskBase_var a_solver_base_task_factory = create_factory< TaskFactoryB_i, TaskFactoryB >( orb, poa, "TaskFactory", "Foam_SolverTaskBase" );
    TaskManager_var a_task_manager = create_factory< TaskManager_i, TaskManager >( orb, poa, "TaskManager", "this" );

#else
    TimeSourceTaskFactory_var a_time_source_task_factory = TimeSourceTaskFactory::_narrow( getObjectReference( orb, "TaskFactory", "Foam_TimeSource" ) );
    //SolverTaskBase_var a_solver_base_task_factory = TaskFactoryB::_narrow( getObjectReference( orb, "TaskFactory", "Foam_SolverTaskBase" ) );
    TaskManager_var a_task_manager = TaskManager::_narrow( getObjectReference( orb, "TaskManager", "this" ) );

#endif

    //---------------------------------------------------------------------------
    TimeSourceTask_var a_time_source_task = a_time_source_task_factory->create( "Foam_TimeSourceTask_launcher" );
    //SolverTaskBase_var a_solver_base_task = a_solver_base_task_factory->create( "Foam_SolverTaskBase_launcher" );

    //a_task_manager->connect( a_time_source_task_factory, "time", a_solver_base_task, "time" );
    
    a_time_source_task->Release();
    //a_solver_base_task->Release();

    a_task_manager->run();

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
