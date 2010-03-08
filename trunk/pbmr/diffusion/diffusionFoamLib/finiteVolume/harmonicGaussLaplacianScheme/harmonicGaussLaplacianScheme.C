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
    
\*---------------------------------------------------------------------------*/

#include "harmonicGaussLaplacianScheme.H"
#include "harmonic.H"
#include "fvcDiv.H"
#include "fvcGrad.H"
#include "fvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void harmonicGaussLaplacianScheme<Type>::calculateTensorCoeffs
(
    const volTensorField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    surfaceScalarField& gammaMagSf,
    GeometricField<Type, fvsPatchField, surfaceMesh>& gammaCorr
)
{
    notImplemented
    (
        "template<class Type>"
        "void calculateTensorCoeffs"
        "("
        "const volTensorField& gamma,"
        "const GeometricField<Type, fvPatchField, volMesh>& vf,"
        "surfaceScalarField& gammaMagSf,"
        "GeometricField<Type, fvsPatchField, surfaceMesh>& gammaCorr"
        ")"
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
harmonicGaussLaplacianScheme<Type>::fvmLaplacianUncorrected
(
    const surfaceScalarField& gammaMagSf,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<surfaceScalarField> tdeltaCoeffs =
        this->tsnGradScheme_().deltaCoeffs(vf);
    const surfaceScalarField& deltaCoeffs = tdeltaCoeffs();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            deltaCoeffs.dimensions()*gammaMagSf.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.upper() = deltaCoeffs.internalField()*gammaMagSf.internalField();
    fvm.negSumDiag();

    forAll(fvm.psi().boundaryField(), patchI)
    {
        const fvPatchField<Type>& psf = fvm.psi().boundaryField()[patchI];
        const fvsPatchScalarField& patchGamma =
            gammaMagSf.boundaryField()[patchI];

        fvm.internalCoeffs()[patchI] = patchGamma*psf.gradientInternalCoeffs();
        fvm.boundaryCoeffs()[patchI] = -patchGamma*psf.gradientBoundaryCoeffs();
    }

    return tfvm;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
harmonicGaussLaplacianScheme<Type>::fvmLaplacian
(
    const surfaceScalarField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    notImplemented
    (
        "harmonicGaussLaplacianScheme<Type>::fvmLaplacian("
        "const surfaceScalarField& gamma,"
        "GeometricField<Type, fvPatchField, volMesh>& vf)"
    );

    return tmp<fvMatrix<Type> >(NULL);
}

template<class Type>
tmp<fvMatrix<Type> >
harmonicGaussLaplacianScheme<Type>::fvmLaplacian
(
    const volScalarField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceScalarField gammaMagSf
        = harmonic(mesh).interpolate(gamma)*mesh.magSf();

    tmp<fvMatrix<Type> > tfvm = fvmLaplacianUncorrected(gammaMagSf, vf);
    fvMatrix<Type>& fvm = tfvm();

    if (this->tsnGradScheme_().corrected())
    {
        if (mesh.fluxRequired(vf.name()))
        {
            fvm.faceFluxCorrectionPtr() = new
            GeometricField<Type, fvsPatchField, surfaceMesh>
            (
                gammaMagSf*this->tsnGradScheme_().correction(vf)
            );

            fvm.source() -=
                mesh.V()*
                fvc::div
                (
                    *fvm.faceFluxCorrectionPtr()
                )().internalField();
        }
        else
        {
            fvm.source() -=
                mesh.V()*
                fvc::div
                (
                    gammaMagSf*this->tsnGradScheme_().correction(vf)
                )().internalField();
        }
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
harmonicGaussLaplacianScheme<Type>::fvmLaplacian
(
    const surfaceTensorField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    notImplemented
    (
        "harmonicGaussLaplacianScheme<Type>::fvmLaplacian("
        "const surfaceTensorField& gamma,"
        "GeometricField<Type, fvPatchField, volMesh>& vf)"
    );

    return tmp<fvMatrix<Type> >(NULL);
}


template<class Type>
tmp<fvMatrix<Type> >
harmonicGaussLaplacianScheme<Type>::fvmLaplacian
(
    const volTensorField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceScalarField gammaMagSf
    (
        IOobject
        (
            "gammaMagSf",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        gamma.dimensions()*dimArea
    );

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tGammaCorr
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "gammaCorr",
                mesh.time().timeName(),
                mesh
            ),
            mesh,
            gamma.dimensions()*vf.dimensions()*dimArea/dimLength
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& gammaCorr = tGammaCorr();

    calculateTensorCoeffs(gamma, vf, gammaMagSf, gammaCorr);

    tmp<fvMatrix<Type> > tfvm = fvmLaplacianUncorrected(gammaMagSf, vf);
    fvMatrix<Type>& fvm = tfvm();

    if (this->tsnGradScheme_().corrected())
    {
        gammaCorr += gammaMagSf*this->tsnGradScheme_().correction(vf);
    }

    fvm.source() -= mesh.V()*fvc::div(gammaCorr)().internalField();

    if (mesh.fluxRequired(vf.name()))
    {
        fvm.faceFluxCorrectionPtr() = tGammaCorr.ptr();
    }

    return tfvm;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
harmonicGaussLaplacianScheme<Type>::fvcLaplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div(this->tsnGradScheme_().snGrad(vf)*mesh.magSf())
    );

    tLaplacian().rename("laplacian(" + vf.name() + ')');

    return tLaplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
harmonicGaussLaplacianScheme<Type>::fvcLaplacian
(
    const surfaceScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    notImplemented
    (
        "harmonicGaussLaplacianScheme<Type>::fvcLaplacian("
        "const surfaceScalarField& gamma,"
        "GeometricField<Type, fvPatchField, volMesh>& vf)"
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >(NULL);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
harmonicGaussLaplacianScheme<Type>::fvcLaplacian
(
    const volScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div
        (
            harmonic(mesh).interpolate(gamma)
          * this->tsnGradScheme_().snGrad(vf)*mesh.magSf()
        )
    );

    tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');

    return tLaplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
harmonicGaussLaplacianScheme<Type>::fvcLaplacian
(
    const surfaceTensorField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    notImplemented
    (
        "harmonicGaussLaplacianScheme<Type>::fvcLaplacian("
        "const surfaceTensorField& gamma,"
        "GeometricField<Type, fvPatchField, volMesh>& vf)"
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >(NULL);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
harmonicGaussLaplacianScheme<Type>::fvcLaplacian
(
    const volTensorField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceScalarField gammaMagSf
    (
        IOobject
        (
            "gammaMagSf",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        gamma.dimensions()*dimArea
    );

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tGammaCorr
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "gammaCorr",
                mesh.time().timeName(),
                mesh
            ),
            mesh,
            gamma.dimensions()*vf.dimensions()*dimArea/dimLength
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& gammaCorr = tGammaCorr();

    calculateTensorCoeffs(gamma, vf, gammaMagSf, gammaCorr);

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div
        (
            gammaMagSf*this->tsnGradScheme_().snGrad(vf)
          + gammaCorr
        )
    );

    tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');

    return tLaplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
