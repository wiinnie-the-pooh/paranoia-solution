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
#include "parallel/foam/diffusion/TNuclearSolver.h"

#include <nuclearCalculation.H>

#include <nuclearField.H>
#include <heatProduction.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    TNuclearSolver::TNuclearSolver( const TimePtr& runTime, const fvMeshPtr& mesh, const Switch& isTransient )
      : device( new diffusionFoam::nuclearCalculation( *mesh, isTransient ) )
    {
      this->runTime = runTime;
      this->mesh = mesh;
        
      this->Tmod = tmp< volScalarField >( new volScalarField( IOobject( word( "Tmod" ),
                                                                        fileName( mesh->time().timeName() ),
                                                                        *mesh,
                                                                        IOobject::MUST_READ,
                                                                        IOobject::AUTO_WRITE ),
                                                              *mesh ) );
      
      this->Tfuel = tmp< volScalarField >( new volScalarField( IOobject( word( "Tfuel" ),
                                                                         fileName( mesh->time().timeName() ),
                                                                         *mesh,
                                                                         IOobject::MUST_READ,
                                                                         IOobject::AUTO_WRITE ),
                                                               *mesh ) );
    }
    
    
    //-----------------------------------------------------------------------
    TNuclearSolver* TNuclearSolver::create( const argList& args, const Switch& isTransient )
    {
      TimePtr runTime = createTime( args.path(), "nuclear" );
      fvMeshPtr mesh = createMesh( *runTime );
      
      return new TNuclearSolver( runTime, mesh, isTransient );
    }
    
    
    //-----------------------------------------------------------------------
    TNuclearSolver::~TNuclearSolver()
    {}


    //-----------------------------------------------------------------------
    const volScalarField& TNuclearSolver::powerDensity() const
    {
      const diffusionFoam::nuclearField& aNuclearField = this->device->nuclField();
      const diffusionFoam::heatProduction& aHeatProduction = aNuclearField.power();

      return aHeatProduction.powerDensity();
    }


    //-----------------------------------------------------------------------
    scalar TNuclearSolver::solve()
    {
      this->device->storeOldTime();
      
      scalar residual = this->device->solve();
      
      this->device->writeScalars();
      
      return residual;
    }


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
