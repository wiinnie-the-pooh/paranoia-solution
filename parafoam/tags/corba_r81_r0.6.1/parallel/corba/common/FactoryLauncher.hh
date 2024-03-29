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
#ifndef corba_common_FactoryLauncher_hh
#define corba_common_FactoryLauncher_hh


//---------------------------------------------------------------------------
#include "parallel/corba/common/corba_utilities.hh"

#include <CORBA.h>

#include <string>
#include <iostream>


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  template< class FactoryImplType, class FactoryType >
  typename FactoryType::_var_type
  create_factory( const CORBA::ORB_var& theORB, 
                  const PortableServer::POA_var& thePOA, 
                  const std::string& theObjectType, 
                  const std::string& theObjectName )
  {
    FactoryImplType* a_factory_i = new FactoryImplType( theORB, thePOA );
    typename FactoryType::_var_type a_factory_ref = a_factory_i->_this();
    
    if( ! bindObjectToName( theORB, a_factory_ref, theObjectType, theObjectName ) )
      FactoryType::_nil();

    return a_factory_ref;
  }


  //---------------------------------------------------------------------------
  template< class FactoryImplType, class FactoryType >
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
  
      create_factory< FactoryImplType, FactoryType >( orb, poa, theObjectType, theObjectName );
  
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
