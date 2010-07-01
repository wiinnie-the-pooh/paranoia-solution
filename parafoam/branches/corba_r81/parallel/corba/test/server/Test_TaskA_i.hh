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
#ifndef corba_server_Test_TaskA_i_hh
#define corba_server_Test_TaskA_i_hh


//---------------------------------------------------------------------------
#include "Test_TaskFactoryA.hh"

#include "parallel/corba/server/TaskBase_i.hh"

#include "parallel/corba/server/PortBool_i.hh"

#include "parallel/corba/server/SPortHelperBase.hh"

#include "parallel/corba/server/SerializedPortInt_i.hh"


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  namespace test
  {
    //---------------------------------------------------------------------------
    struct TaskA_i : virtual POA_parallel::test::TaskA, virtual TaskBase_i
    {
      TaskA_i( const CORBA::ORB_var& theORB, 
               const PortableServer::POA_var& thePOA );
      
      ~TaskA_i();
      
      virtual void prepare() 
      {}

    protected:
      virtual bool step();
      
      virtual void destroy() 
      {}
      
    protected:
      SPortHelperBase< PortBool_i > m_x;
      
      SPortHelperBase< SerializedPortInt_i > m_sx;
    };


    //---------------------------------------------------------------------------
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
