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
#ifndef corba_server_Foam_Diffusion_NuclearSolverTaskFactory_i_hh
#define corba_server_Foam_Diffusion_NuclearSolverTaskFactory_i_hh


//---------------------------------------------------------------------------
#include "Foam_Diffusion_NuclearSolverTaskFactory.hh"

#include "parallel/corba/server/TaskFactoryBase_i.hh"


//---------------------------------------------------------------------------
#ifdef __USE_CORBA_SINGLE_PROCESS__
#include "parallel/corba/foam/diffusion/server/Foam_Diffusion_NuclearSolverTask_i.hh"
#endif


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  namespace foam
  {
    //---------------------------------------------------------------------------
    namespace diffusion
    {
      //---------------------------------------------------------------------------
#ifndef __USE_CORBA_SINGLE_PROCESS__
      typedef TaskFactoryBase_i< POA_parallel::foam::diffusion::NuclearSolverTaskFactory, NuclearSolverTask > NuclearSolverTaskFactory_i;
#else
  typedef TaskFactoryBase_i< POA_parallel::foam::NuclearSolverTaskFactory, NuclearSolverTask, NuclearSolverTask_i, NuclearSolverTaskFactory > NuclearSolverTaskFactory_i;
#endif


      //---------------------------------------------------------------------------
    }


    //---------------------------------------------------------------------------
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
