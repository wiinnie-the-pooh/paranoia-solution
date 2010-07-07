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
#ifndef corba_server_SolverBaseTask_i_hh
#define corba_server_SolverBaseTask_i_hh


//---------------------------------------------------------------------------
#include "parallel/foam/corba/idl/SolverBaseTask.hh"

#include "parallel/corba/server/TaskBase_i.hh"

#include "parallel/corba/server/PortBool_i.hh"

#include "parallel/corba/server/PortInt_i.hh"

#include "parallel/foam/corba/server/DimensionedScalarPort_i.hh"

#include "parallel/corba/server/PortFloat_i.hh"

#include "parallel/corba/server/SPortHelperBase.hh"


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  namespace foam
  {
    //---------------------------------------------------------------------------
    struct SolverBaseTask_i : virtual POA_parallel::foam::SolverBaseTask, 
                              virtual TaskBase_i
    {
      SolverBaseTask_i( const CORBA::ORB_var& theORB, 
                        const PortableServer::POA_var& thePOA );
      
      ~SolverBaseTask_i();
      
    protected:
      bool pre_step();
      bool post_step();

    protected:
      SPortHelperBase< DimensionedScalarPort_i > m_time_i;
      SPortHelperBase< PortInt_i > m_index_i;
      SPortHelperBase< PortBool_i > m_write_i;
      SPortHelperBase< PortBool_i > m_stop_i;

      SPortHelperBase< PortBool_i > m_finished_o;
      SPortHelperBase< PortFloat_i > m_residual_o;
    };


    //---------------------------------------------------------------------------
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
