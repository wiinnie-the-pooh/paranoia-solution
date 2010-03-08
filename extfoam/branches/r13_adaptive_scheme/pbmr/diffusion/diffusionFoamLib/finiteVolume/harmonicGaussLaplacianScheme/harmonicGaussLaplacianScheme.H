/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    harmonicGaussLaplacianScheme

Description
    Basic second-order laplacian using face-gradients and Gauss' theorem.
    Based on GaussLaplacianScheme but specifically uses harmonic
    interpolation for scalars. For tensors harmonic interpolation is used
    for the orthogonal contribution and linear interpolation for the
    non-orthogonal contribution. Linear interpolation is used for the field
    gradient term

SourceFiles
    harmonicGaussLaplacianScheme.C

\*---------------------------------------------------------------------------*/

#ifndef harmonicGaussLaplacianScheme_H
#define harmonicGaussLaplacianScheme_H

#include "laplacianScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class gaussLaplacianScheme Declaration
\*---------------------------------------------------------------------------*/
//- Basic second-order laplacian using face-gradients and Gauss' theorem.
//- Based on GaussLaplacianScheme but specifically uses harmonic
//- interpolation for scalars. For tensors harmonic interpolation is used
//- for the orthogonal contribution and linear interpolation for the
//- non-orthogonal contribution. Linear interpolation is used for the field
//- gradient term
template<class Type>
class harmonicGaussLaplacianScheme
:
    public fv::laplacianScheme<Type>
{
    // Private Member Functions

        void calculateTensorCoeffs
        (
            const volTensorField& gamma,
            const GeometricField<Type, fvPatchField, volMesh>& vf,
            surfaceScalarField& gammaMagSf,
            GeometricField<Type, fvsPatchField, surfaceMesh>& gammaCorr
        );

        tmp<fvMatrix<Type> > fvmLaplacianUncorrected
        (
            const surfaceScalarField& gammaMagSf,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        //- Disallow default bitwise copy construct
        harmonicGaussLaplacianScheme(const harmonicGaussLaplacianScheme&);

        //- Disallow default bitwise assignment
        void operator=(const harmonicGaussLaplacianScheme&);


public:

    //- Runtime type information
    TypeName("harmonicGauss");


    // Constructors

        //- Construct null
        harmonicGaussLaplacianScheme(const fvMesh& mesh)
        :
            laplacianScheme<Type>(mesh)
        {}

        //- Construct from Istream
        harmonicGaussLaplacianScheme(const fvMesh& mesh, Istream& is)
        :
            laplacianScheme<Type>(mesh, is)
        {}

        //- Construct from mesh, interpolation and snGradScheme schemes
        harmonicGaussLaplacianScheme
        (
            const fvMesh& mesh,
            const tmp<surfaceInterpolationScheme<scalar> >& igs,
            const tmp<snGradScheme<Type> >& sngs
        )
        :
            laplacianScheme<Type>(mesh, igs, sngs)
        {}


    // Destructor

        virtual ~harmonicGaussLaplacianScheme()
        {}


    // Member Functions

        tmp<fvMatrix<Type> > fvmLaplacian
        (
            const volScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmLaplacian
        (
            const volTensorField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmLaplacian
        (
            const surfaceScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmLaplacian
        (
            const surfaceTensorField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const surfaceScalarField& gamma,
            const GeometricField<Type, fvPatchField, volMesh>& vf
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const surfaceTensorField& gamma,
            const GeometricField<Type, fvPatchField, volMesh>& vf
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const volTensorField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "harmonicGaussLaplacianScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
