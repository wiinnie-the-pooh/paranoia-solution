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
#ifndef TNuclearSolverTask_h
#define TNuclearSolverTask_h


//---------------------------------------------------------------------------
#include "parallel/foam/CSolverTaskBase.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/foam/CSerializedFvMesh.h"

#include "parallel/foam/CGeometricFieldPortHelper.h"

#include <argList.H>
#include <Switch.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    using namespace Foam;


    //---------------------------------------------------------------------------
    struct TNuclearSolver;
    typedef Loki::SmartPtrDef< TNuclearSolver >::type TNuclearSolverPtr;


    //---------------------------------------------------------------------------
    struct TNuclearSolverTask : CSolverTaskBase
    {
      typedef base::SmartPtrDef< TNuclearSolverTask >::type TPtr;

      static TPtr create( const argList& args, const Switch& isTransient );

      ~TNuclearSolverTask();

    protected:
      TNuclearSolverPtr engine;

      CGeometricFieldPortHelper< Foam::volScalarField > m_Tfuel_i;
      CGeometricFieldPortHelper< Foam::volScalarField > m_Tmod_i;
      dev::CPortHelper< CSerializedFvMesh > m_fvMesh_o;
      CGeometricFieldPortHelper< Foam::volScalarField > m_powerDensity_o;

   protected:
      TNuclearSolverTask( const TNuclearSolverPtr& engine );

      virtual void init();
      virtual bool step();
      virtual void destroy();
    };


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
