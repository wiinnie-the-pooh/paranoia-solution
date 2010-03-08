// Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License. 
// 
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details. 
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#ifndef TTemperatureSolverTask_h
#define TTemperatureSolverTask_h


//---------------------------------------------------------------------------
#include "parallel/foam/CSolverTaskBase.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/foam/CFvMeshPort.h"

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
    struct TTemperatureSolver;
    typedef Loki::SmartPtrDef< TTemperatureSolver >::type TTemperatureSolverPtr;


    //---------------------------------------------------------------------------
    struct TTemperatureSolverTask : CSolverTaskBase
    {
      typedef base::SmartPtrDef< TTemperatureSolverTask >::type TPtr;

      static TPtr create( const argList& args, const Switch& isTransient );
      
      ~TTemperatureSolverTask();
      
    protected:
      TTemperatureSolverPtr engine;

      CGeometricFieldPortHelper< Foam::volScalarField > m_powerDensity_i;
      dev::CPortHelper< CFvMeshPort > m_fvMesh_o;
      CGeometricFieldPortHelper< Foam::volScalarField > m_Tfuel_o;
      CGeometricFieldPortHelper< Foam::volScalarField > m_Tmod_o;

    protected:
      TTemperatureSolverTask( const TTemperatureSolverPtr& engine );

      virtual void init();
      virtual bool step();
      virtual void destroy();
    };


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
