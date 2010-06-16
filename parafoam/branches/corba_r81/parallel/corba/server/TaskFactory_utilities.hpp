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
#ifndef corba_server_TaskFactory_utilities_hpp
#define corba_server_TaskFactory_utilities_hpp


//---------------------------------------------------------------------------
#include <CORBA.h>

#include <string>
#include <iostream>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  CORBA::Boolean bindObjectToName( CORBA::ORB_ptr orb,
				   CORBA::Object_ptr objref, 
				   const std::string& theTaskFactoryName );


  //---------------------------------------------------------------------------
  template< class TaskFactoryType, template< class > class TaskFactoryTieType >
  int run( int argc, char** argv, const std::string& theTaskFactoryName )
  {
    try {
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );
  
      CORBA::Object_var poa_obj = orb->resolve_initial_references( "RootPOA" );
      PortableServer::POA_var poa = PortableServer::POA::_narrow( poa_obj );
  
      TaskFactoryType* a_task_factory_impl = new TaskFactoryType();
      TaskFactoryTieType< TaskFactoryType > a_task_factory_tie( a_task_factory_impl );
  
      PortableServer::ObjectId_var a_task_factory_id = poa->activate_object( &a_task_factory_tie );
  
      // Obtain a reference to the object, and register it in the naming service.
      CORBA::Object_var a_task_factory_obj = a_task_factory_tie._this();
      if( !bindObjectToName( orb, a_task_factory_obj, theTaskFactoryName ) )
        return 1;
  
      PortableServer::POAManager_var pman = poa->the_POAManager();
      pman->activate();
  
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
