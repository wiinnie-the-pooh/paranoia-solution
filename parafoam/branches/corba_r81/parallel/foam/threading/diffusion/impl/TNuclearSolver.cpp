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
#include "parallel/foam/threading/diffusion/impl/TNuclearSolver.h"

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
