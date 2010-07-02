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
#ifndef corba_common_TaskLauncher_hh
#define corba_common_TaskLauncher_hh


//---------------------------------------------------------------------------
#include "parallel/corba/common/corba_utilities.hh"

#include <CORBA.h>

#include <string>
#include <iostream>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  template< class TaskImplType, class TaskType, class TaskFactoryType >
  typename TaskType::_var_type 
  create_task( const CORBA::ORB_var& theORB, 
               const PortableServer::POA_var& thePOA,
               const typename TaskFactoryType::_var_type the_task_factory_ref )
  {
    TaskImplType* a_task_i = new TaskImplType( theORB, thePOA );
    typename TaskType::_var_type a_task_ref = a_task_i->_this();
    
    the_task_factory_ref->publish( a_task_ref );

    return a_task_ref;
  }


  //---------------------------------------------------------------------------
  template< class TaskImplType, class TaskType, class TaskFactoryType >
  int run( int argc, 
           char** argv, 
           const std::string& theObjectType, 
           const std::string& theObjectName )
  {
    try {
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );
  
      CORBA::Object_var poa_obj = orb->resolve_initial_references( "RootPOA" );
      PortableServer::POA_var poa = PortableServer::POA::_narrow( poa_obj );
  
      PortableServer::POAManager_var pman = poa->the_POAManager();
      pman->activate();
  
      CORBA::Object_var a_task_factory_obj = getObjectReference( orb, theObjectType, theObjectName );
      typename TaskFactoryType::_var_type a_task_factory_ref = TaskFactoryType::_narrow( a_task_factory_obj );

      create_task< TaskImplType, TaskType, TaskFactoryType >( orb, poa, a_task_factory_ref );
      
      orb->run();
    }
    catch( CORBA::SystemException& ex ) {
      std::cerr << "Caught CORBA::" << ex._name() << std::endl;
    }
    catch( CORBA::Exception& ex ) {
      std::cerr << "Caught CORBA::Exception: " << ex._name() << std::endl;
    }
    catch( omniORB::fatalException& fe ) {
      std::cerr << "Caught omniORB::fatalException:" << std::endl;
      std::cerr << "  file: " << fe.file() << std::endl;
      std::cerr << "  line: " << fe.line() << std::endl;
      std::cerr << "  mesg: " << fe.errmsg() << std::endl;
    }

    return 0;
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
