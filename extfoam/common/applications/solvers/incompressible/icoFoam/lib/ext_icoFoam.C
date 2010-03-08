//  Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
//
//  This code is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://sourceforge.net/projects/pyfoam
//
//  Author : Alexey PETROV


//---------------------------------------------------------------------------
#include "ext_icoFoam.H"

using namespace Foam;


//---------------------------------------------------------------------------
t_get_time::t_get_time( const word& the_dict, const fileName& the_rootPath, const fileName& the_caseName )
  : m_time_ptr( new Time( the_dict, the_rootPath, the_caseName ) )
{} 

t_time_ptr t_get_time::call() const
{ 
  return m_time_ptr;
}
  
  
//---------------------------------------------------------------------------
t_fvMesh_ptr t_get_mesh::call( const Time& the_runTime ) const 
{
  Info << "Create mesh for time = "
       << the_runTime.timeName() << nl << endl;

  return t_fvMesh_ptr( new fvMesh( IOobject( fvMesh::defaultRegion,
					     the_runTime.timeName(),
					     the_runTime,
					     IOobject::MUST_READ
				            )
			          )
                      );
} 
  
  
//---------------------------------------------------------------------------
t_piso_controls t_read_piso_controls::call( const fvMesh& the_mesh ) const 
{
  t_piso_controls a_piso_ctrls;
  
  a_piso_ctrls.piso = the_mesh.solutionDict().subDict("PISO");

  a_piso_ctrls.nCorr = readInt(a_piso_ctrls.piso.lookup("nCorrectors"));

  a_piso_ctrls.nNonOrthCorr = 0;
  if ( a_piso_ctrls.piso.found( "nNonOrthogonalCorrectors" ) )
  {
    a_piso_ctrls.nNonOrthCorr = readInt( a_piso_ctrls.piso.lookup( "nNonOrthogonalCorrectors" ));
  }

  a_piso_ctrls.momentumPredictor = true;
  if ( a_piso_ctrls.piso.found( "momentumPredictor" ) )
  {
    a_piso_ctrls.momentumPredictor = Switch( a_piso_ctrls.piso.lookup( "momentumPredictor" ) );
  }

  a_piso_ctrls.fluxGradp = false;
  if ( a_piso_ctrls.piso.found( "fluxGradp" ) )
  {
    a_piso_ctrls.fluxGradp = Switch( a_piso_ctrls.piso.lookup( "fluxGradp" ) );
  }

  a_piso_ctrls.transSonic = false;
  if ( a_piso_ctrls.piso.found( "transSonic" ) )
  {
    a_piso_ctrls.transSonic = Switch( a_piso_ctrls.piso.lookup( "transSonic" ) );
  }

  a_piso_ctrls.nOuterCorr = 1;
  if ( a_piso_ctrls.piso.found( "nOuterCorrectors" ) )
  {
    a_piso_ctrls.nOuterCorr = readInt(a_piso_ctrls.piso.lookup("nOuterCorrectors"));
  }
  
  return a_piso_ctrls;
}
  
  
//---------------------------------------------------------------------------
t_fields t_get_fields::call( const Time& the_run_time, const fvMesh& the_mesh ) const 
{
  t_fields a_fields;
      
  Info << "Reading transportProperties\n" << endl;

  a_fields.transportProperties_ptr.reset( new IOdictionary( IOobject( "transportProperties",
								      the_run_time.constant(),
								      the_mesh,
								      IOobject::MUST_READ,
								      IOobject::NO_WRITE
								     )
					                   )
                                         );
  IOdictionary& a_transportProperties = *a_fields.transportProperties_ptr;

  // nu
  a_fields.nu_ptr.reset( new dimensionedScalar( a_transportProperties.lookup( "nu" ) ) );

  // p
  Info << "Reading field p\n" << endl;
  a_fields.p_ptr.reset( new volScalarField( IOobject( "p",
						      the_run_time.timeName(),
						      the_mesh,
						      IOobject::MUST_READ,
						      IOobject::AUTO_WRITE
						     ),
					    the_mesh
			                   )
                      );
  volScalarField& a_p = *a_fields.p_ptr;

  //U
  Info << "Reading field U\n" << endl;
  a_fields.U_ptr.reset( new volVectorField( IOobject( "U",
						      the_run_time.timeName(),
						      the_mesh,
						      IOobject::MUST_READ,
						      IOobject::AUTO_WRITE
						     ),
					    the_mesh
			                   )  
                      );
  volVectorField& a_U = *a_fields.U_ptr; 

  //phi
  Info << "Reading/calculating face flux field phi\n" << endl;
  a_fields.phi_ptr.reset( new surfaceScalarField( IOobject( "phi",
							    the_run_time.timeName(),
							    the_mesh,
							    IOobject::READ_IF_PRESENT,
							    IOobject::AUTO_WRITE
						          ),
						  linearInterpolate(a_U) & the_mesh.Sf()
			                        )
                        );

  a_fields.pRefCell = 0;
  a_fields.pRefValue = 0.0;
  setRefCell( a_p, the_mesh.solutionDict().subDict( "PISO" ), a_fields.pRefCell, a_fields.pRefValue );
      
  return a_fields;
}
  
   
//---------------------------------------------------------------------------
icoFoam::icoFoam( t_get_time_base* the_get_time, t_get_mesh_base* the_get_mesh, t_get_fields_base* the_get_fields)
  : cumulativeContErr( 0 )
{
  this->runTime = the_get_time->call();
           
  this->mesh = the_get_mesh->call( *this->runTime );
      
  t_fields a_fields = the_get_fields->call( *this->runTime, *this->mesh );
  this->transportProperties = a_fields.transportProperties_ptr;
  this->nu = a_fields.nu_ptr;
  this->p = a_fields.p_ptr;
  this->U = a_fields.U_ptr;
  this->phi = a_fields.phi_ptr;
  this->pRefCell = a_fields.pRefCell;
  this->pRefValue = a_fields.pRefValue;
};
	    
	   
//---------------------------------------------------------------------------
void icoFoam::step( t_read_piso_controls_base* the_read_piso_controls )
{ 
  t_piso_controls a_piso_ctrls( the_read_piso_controls->call( *this->mesh ) );
      
  int nCorr = a_piso_ctrls.nCorr;
  int nNonOrthCorr = a_piso_ctrls.nNonOrthCorr;
      
  // The main body
  scalar CoNum = 0.0;
  scalar meanCoNum = 0.0;

  if ( this->mesh->nInternalFaces() )
  {
    surfaceScalarField SfUfbyDelta = this->mesh->surfaceInterpolation::deltaCoeffs() * mag( *this->phi );

    CoNum = max( SfUfbyDelta / this->mesh->magSf() ).value() * this->runTime->deltaT().value();
    
    meanCoNum = ( sum( SfUfbyDelta ) / sum( this->mesh->magSf() ) ).value() * this->runTime->deltaT().value();
  }

  Info << "Courant Number mean: " << meanCoNum
       << " max: " << CoNum
       << endl;
  
  fvVectorMatrix UEqn( fvm::ddt( *this->U ) + fvm::div( *this->phi, *this->U ) - fvm::laplacian( *this->nu, *this->U ) );

  solve( UEqn == -fvc::grad( *this->p ) );

  // --- PISO loop

  for ( int corr = 0; corr < nCorr; corr++ )
  {
    volScalarField rUA = 1.0 / UEqn.A();

    *this->U = rUA * UEqn.H();
    *this->phi = ( fvc::interpolate( *this->U ) & this->mesh->Sf() ) + fvc::ddtPhiCorr( rUA, *this->U, *this->phi );

    adjustPhi( *this->phi, *this->U, *this->p );

    for ( int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++ )
    {
      fvScalarMatrix pEqn( fvm::laplacian( rUA, *this->p ) == fvc::div( *this->phi ) );

      pEqn.setReference( this->pRefCell, this->pRefValue );
      pEqn.solve();
      
      if ( nonOrth == nNonOrthCorr )
      {
	*this->phi -= pEqn.flux();
      }
    }
            
    volScalarField contErr = fvc::div( *this->phi );

    scalar sumLocalContErr = this->runTime->deltaT().value() * mag( contErr )().weightedAverage( this->mesh->V() ).value();

    scalar globalContErr = this->runTime->deltaT().value() * contErr.weightedAverage( this->mesh->V() ).value();
            
    this->cumulativeContErr += globalContErr;

    Info << "time step continuity errors : sum local = " << sumLocalContErr
	 << ", global = " << globalContErr
	 << ", cumulative = " << this->cumulativeContErr
	 << endl;
    
    *this->U -= rUA * fvc::grad( *this->p );
    this->U->correctBoundaryConditions();
  }

  this->runTime->write();

  Info << "ExecutionTime = " << this->runTime->elapsedCpuTime() << " s"
       << "  ClockTime = " << this->runTime->elapsedClockTime() << " s"
       << nl << endl;
}
	    
    
//---------------------------------------------------------------------------
void icoFoam::run( t_read_piso_controls_base* the_read_piso_controls )
{
  Info<< "\nStarting time loop\n" << endl;
  for ( (*this->runTime)++; !this->runTime->end(); (*this->runTime)++ )
  {
    this->step( the_read_piso_controls );
  }
}
	    
    
//---------------------------------------------------------------------------
