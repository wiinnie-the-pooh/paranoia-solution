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

#include "parallel/corba/common/corba_utilities.hh"

#include "parallel/corba/CORBASmartPtr.hh"

#include <iostream>
using namespace std;


//---------------------------------------------------------------------------
int main( int argc, char **argv ) 
{
  try {
    using namespace parallel;

    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    CORBA::Object_var a_task_factory_A_obj = getObjectReference( orb, "TaskFactory", "A" );
    TaskFactoryA_var a_task_factory_A_ref = TaskFactoryA::_narrow( a_task_factory_A_obj );
    TaskA_var a_task_A = a_task_factory_A_ref->create( "TaskA_launcher" );

    CORBA::Object_var a_task_factory_B_obj = getObjectReference( orb, "TaskFactory", "B" );
    TaskFactoryB_var a_task_factory_B_ref = TaskFactoryB::_narrow( a_task_factory_B_obj );
    TaskB_var a_task_B = a_task_factory_B_ref->create( "TaskB_launcher" );

    CORBA::Object_var a_task_manager_obj = getObjectReference( orb, "TaskManager", "this" );
    TaskManager_var a_task_manager_ref = TaskManager::_narrow( a_task_manager_obj );

    //a_task_manager_ref->connect( a_task_A, "x", a_task_B, "y" );
    a_task_manager_ref->connect( a_task_A, "sx", a_task_B, "sy" );
    
    a_task_manager_ref->run();

    //{
    //  corba::SmartPtrDef< TaskA_var >::type a_task_A( a_task_factory_A_ref->create( "localhost" ) );
    //  if ( a_task_A ) 
    //        return 0;
    //}

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
