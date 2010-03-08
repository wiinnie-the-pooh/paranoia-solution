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
