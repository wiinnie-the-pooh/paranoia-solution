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
#include "Test_TaskFactoryA.hh"
#include "Test_TaskFactoryB.hh"
#include "TaskManager.hh"

#include "parallel/corba/common/corba_utilities.hh"

#include "parallel/corba/CORBASmartPtr.hh"


//---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__

#include "parallel/corba/server/TaskManager_i.hh"
#include "parallel/corba/test/server/Test_TaskFactoryA_i.hh"
#include "parallel/corba/test/server/Test_TaskFactoryB_i.hh"

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
    using namespace parallel::test;

    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );
  
    //---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__
    CORBA::Object_var poa_obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( poa_obj );
    
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();
    
    TaskManager_var a_task_manager_ref = create_factory< TaskManager_i, TaskManager >( orb, poa, "TaskManager", "this" );
    TaskFactoryA_var a_task_factory_A_ref = create_factory< TaskFactoryA_i, TaskFactoryA >( orb, poa, "TaskFactory", "A" );
    TaskFactoryB_var a_task_factory_B_ref = create_factory< TaskFactoryB_i, TaskFactoryB >( orb, poa, "TaskFactory", "B" );

#else
    TaskFactoryA_var a_task_factory_A_ref = TaskFactoryA::_narrow( getObjectReference( orb, "TaskFactory", "A" ) );
    TaskFactoryB_var a_task_factory_B_ref = TaskFactoryB::_narrow( getObjectReference( orb, "TaskFactory", "B" ) );
    TaskManager_var a_task_manager_ref = TaskManager::_narrow( getObjectReference( orb, "TaskManager", "this" ) );

#endif

    //---------------------------------------------------------------------------
    TaskA_var a_task_A = a_task_factory_A_ref->create( "TaskA_launcher" );
    TaskB_var a_task_B = a_task_factory_B_ref->create( "TaskB_launcher" );

    a_task_manager_ref->connect( a_task_A, "x", a_task_B, "y" );
    a_task_manager_ref->connect( a_task_A, "sx", a_task_B, "sy" );
    
    a_task_A->Release();
    a_task_B->Release();

    a_task_manager_ref->start();

    a_task_manager_ref->wait();

    //{
    //  corba::SmartPtrDef< TaskA_var >::type a_task_A( a_task_factory_A_ref->create( "localhost" ) );
    //  if ( a_task_A ) 
    //        return 0;
    //}

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
