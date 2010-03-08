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
#ifndef ext_icoFoam_H
#define ext_icoFoam_H


//---------------------------------------------------------------------------
#include "fvCFD.H"

#include <boost/shared_ptr.hpp>


//---------------------------------------------------------------------------
namespace Foam
{
  //---------------------------------------------------------------------------
  typedef boost::shared_ptr< Foam::Time > t_time_ptr;
  typedef boost::shared_ptr< Foam::fvMesh > t_fvMesh_ptr;
  typedef boost::shared_ptr< Foam::IOdictionary > t_IOdictionary_ptr;
  typedef boost::shared_ptr< Foam::volScalarField > t_volScalarField_ptr;
  typedef boost::shared_ptr< Foam::volVectorField > t_volVectorField_ptr;
  typedef boost::shared_ptr< Foam::surfaceScalarField > t_surfaceScalarField_ptr;
  typedef boost::shared_ptr< Foam::dimensionedScalar > t_dimensionedScalar_ptr;


  //---------------------------------------------------------------------------
  struct t_piso_controls
  {
    dictionary piso;
    int nCorr;
    int nNonOrthCorr;
    bool momentumPredictor;
    bool fluxGradp;
    bool transSonic;
    int nOuterCorr;
  };

    
  //---------------------------------------------------------------------------
  struct t_fields
  {
    t_IOdictionary_ptr transportProperties_ptr;
    t_volScalarField_ptr p_ptr;
    t_volVectorField_ptr U_ptr;
    t_surfaceScalarField_ptr phi_ptr;
    t_dimensionedScalar_ptr nu_ptr;
    label pRefCell;
    scalar pRefValue;
  };
  

  //---------------------------------------------------------------------------
  struct t_common_base 
  {
    virtual ~t_common_base() {}
  };
  
  struct t_get_time_base : virtual t_common_base
  {
    virtual t_time_ptr call() const = 0;
  };
  
  struct t_get_mesh_base : virtual t_common_base
  {
    virtual t_fvMesh_ptr call( const Time& the_runTime ) const = 0; 
  };

  struct t_read_piso_controls_base : virtual t_common_base
  {
    virtual t_piso_controls call( const fvMesh& the_mesh ) const = 0;
  };
  
  struct t_get_fields_base : virtual t_common_base
  {
    virtual t_fields call( const Time& the_run_time, const fvMesh& the_mesh ) const = 0;
  };
  

  //---------------------------------------------------------------------------
  struct t_get_time: virtual t_get_time_base
  {
    t_get_time( const word& the_dict, const fileName& the_rootPath, const fileName& the_caseName );
      
    virtual t_time_ptr call() const;

  private:
    t_time_ptr m_time_ptr;
  };
  
  
  //---------------------------------------------------------------------------
  struct t_get_mesh: virtual t_get_mesh_base
  {
    virtual t_fvMesh_ptr call( const Time& the_runTime ) const;
  };
  
  
  //---------------------------------------------------------------------------
  struct t_read_piso_controls: virtual t_read_piso_controls_base
  {
    virtual t_piso_controls call( const fvMesh& the_mesh ) const;
  };
  
  
  //---------------------------------------------------------------------------
  struct t_get_fields: virtual t_get_fields_base
  {
    virtual t_fields call( const Time& the_run_time, const fvMesh& the_mesh ) const;
  };
  
    
  //---------------------------------------------------------------------------
  struct icoFoam
  {
    icoFoam( t_get_time_base* the_get_time, t_get_mesh_base* the_get_mesh, t_get_fields_base* the_get_fields );
	   
    void step( t_read_piso_controls_base* the_read_piso_controls );
    
    void run( t_read_piso_controls_base* the_read_piso_controls );
    
  private:
    t_time_ptr runTime;
    t_fvMesh_ptr mesh;

    t_IOdictionary_ptr transportProperties;
    t_dimensionedScalar_ptr nu;
    t_volScalarField_ptr p;
    t_volVectorField_ptr U;
    t_surfaceScalarField_ptr phi;
    label pRefCell;
    scalar pRefValue;

    scalar cumulativeContErr;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif



