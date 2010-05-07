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
#include "parallel/foam/mapConsistentField.h"

#include "parallel/foam/SFoamMutex.h"

#include <meshToMesh.H>


//---------------------------------------------------------------------------
namespace parallel 
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    template< class TGeometricField > 
    void
    print_interpolate_report( const TGeometricField & theTargetField, 
                              const TGeometricField & theSourceField )
    {
      const word& aFieldName = theSourceField.name();
      const fvMesh& meshSource = theSourceField.mesh();
      Info << tab <<"interpolating \"" << aFieldName << "\"" << nl;
      Info << tab << tab << "source : " << meshSource.time().path() << nl;
      Info << tab << tab << tab << fvc::domainIntegrate( theSourceField ) << nl;
      Info << tab << tab << tab << min( theSourceField ) << nl;
      Info << tab << tab << tab << max( theSourceField ) << nl;

      const fvMesh& meshTarget = theTargetField.mesh();
      Info << tab << tab << "source : " << meshTarget.time().path() << nl;
      Info << tab << tab << tab << fvc::domainIntegrate( theTargetField ) << nl;
      Info << tab << tab << tab << min( theTargetField ) << nl;
      Info << tab << tab << tab << max( theTargetField ) << nl;
    }


    //-----------------------------------------------------------------------
    template< class TGeometricField > 
    tmp< TGeometricField >
    mapConsistentVolField( const TGeometricField& theSourceField,
                           const fvMesh& meshTarget, 
                           const HashTable< word >& patchMap,
                           const wordList& cuttingPatches )
    {
      typedef tmp< TGeometricField > TResult;
      
      const fvMesh& meshSource = theSourceField.mesh();
      
      // Create the interpolation scheme
      meshToMesh meshToMeshInterp( meshSource,
                                   meshTarget,
                                   patchMap,
                                   cuttingPatches );
      
      const word& aFieldName = theSourceField.name();
      
      if ( meshTarget.foundObject< TGeometricField >( aFieldName ) ) {
        TResult aTargetField( meshTarget.lookupObject< TGeometricField >( aFieldName ) );

        meshToMeshInterp.interpolate( aTargetField(), theSourceField, meshToMesh::INTERPOLATE );
        print_interpolate_report( aTargetField(), theSourceField );

        return aTargetField;
      }
      
      autoPtr< IOobject > aTargetIOobject;
      {
        SFoamMutex aMutex;
        aTargetIOobject = autoPtr< IOobject >( new IOobject( aFieldName,
                                                             fileName( meshTarget.time().timeName() ),
                                                             meshTarget,
                                                             IOobject::READ_IF_PRESENT,
                                                             IOobject::AUTO_WRITE ) );
      }
      TResult aTargetField;
      
      if ( aTargetIOobject().headerOk() ) {
        {
	  SFoamMutex aMutex;
	  aTargetField = TResult( new TGeometricField( aTargetIOobject(), meshTarget ) );
        }
        meshToMeshInterp.interpolate( aTargetField(), theSourceField, meshToMesh::INTERPOLATE );
      } else {
        aTargetIOobject().readOpt() = IOobject::NO_READ;
        TResult aPrototype = meshToMeshInterp.interpolate( theSourceField, meshToMesh::INTERPOLATE );
        {
	  SFoamMutex aMutex;
	  aTargetField = TResult( new TGeometricField( aTargetIOobject(), aPrototype ) );
        }
      }
      
      print_interpolate_report( aTargetField(), theSourceField );
      
      return aTargetField;
    }
    
    
    //-----------------------------------------------------------------------
    tmp< volScalarField >
    mapConsistentField( const volScalarField& theSourceField,
                        const fvMesh& meshTarget, 
                        const HashTable< word >& patchMap,
                        const wordList& cuttingPatches )
    {
      return mapConsistentVolField( theSourceField, meshTarget, patchMap, cuttingPatches );
    }


    //-----------------------------------------------------------------------
    tmp< volVectorField >
    mapConsistentVolField( const volVectorField& theSourceField,
                           const fvMesh& meshTarget, 
                           const HashTable< word >& patchMap,
                           const wordList& cuttingPatches )
    {
      return mapConsistentVolField( theSourceField, meshTarget, patchMap, cuttingPatches );
    }


    //-----------------------------------------------------------------------
    tmp< volTensorField >
    mapConsistentVolField( const volTensorField& theSourceField,
                           const fvMesh& meshTarget, 
                           const HashTable< word >& patchMap,
                           const wordList& cuttingPatches )
    {
      return mapConsistentVolField( theSourceField, meshTarget, patchMap, cuttingPatches );
    }

    
    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
