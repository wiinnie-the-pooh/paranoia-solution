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
#include "parallel/foam/diffusion/TTemperatureSolver.h"

#include <temperatureCalculation.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    TTemperatureSolver::TTemperatureSolver( const TimePtr& runTime, const fvMeshPtr& mesh, const Switch& isTransient )
      : device( new diffusionFoam::temperatureCalculation( *mesh, isTransient ) )
    {
      this->runTime = runTime;
      this->mesh = mesh;
      
      this->powerDensity = tmp< volScalarField >( new volScalarField( IOobject( word( "Q'''" ),
                                                                                fileName( mesh->time().timeName() ),
                                                                                *mesh,
                                                                                IOobject::MUST_READ,
                                                                                IOobject::AUTO_WRITE ),
                                                                      *mesh ) );
    }
    
    
    //-----------------------------------------------------------------------
    TTemperatureSolver* TTemperatureSolver::create( const argList& args, const Switch& isTransient )
    {
      TimePtr runTime = createTime( args.path(), "temperature" );
      fvMeshPtr mesh = createMesh( *runTime );
      
      return new TTemperatureSolver( runTime, mesh, isTransient );
    }
    
    
    //-----------------------------------------------------------------------
    TTemperatureSolver::~TTemperatureSolver()
    {}
    
    
    //-----------------------------------------------------------------------
    const volScalarField& TTemperatureSolver::Tfuel() const
    {
      return this->device->Tfuel();
    }
    
    
    //-----------------------------------------------------------------------
    const volScalarField& TTemperatureSolver::Tmod() const
    {
      return this->device->Tmod();
    }
    
    
    //-----------------------------------------------------------------------
    scalar TTemperatureSolver::solve()
    {
      scalar residual = this->device->solve();
      
      return residual;
    }
    
    
    //-----------------------------------------------------------------------
  }
}  
 
 
//---------------------------------------------------------------------------
