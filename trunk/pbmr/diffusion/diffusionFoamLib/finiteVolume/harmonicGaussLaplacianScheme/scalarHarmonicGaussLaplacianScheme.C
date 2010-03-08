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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<>
void harmonicGaussLaplacianScheme<scalar>::calculateTensorCoeffs
(
    const volTensorField& gamma,
    const volScalarField& vf,
    surfaceScalarField& gammaMagSf,
    surfaceScalarField& gammaCorr
)
{
    const fvMesh& mesh = this->mesh();

    const volVectorField gradVf = fvc::grad(vf);

    const surfaceVectorField& Sf = mesh.Sf();
    const surfaceScalarField& magSf = mesh.magSf();
    const surfaceScalarField& weights = mesh.weights();
    const labelList& owner = mesh.owner();
    const labelList& neigh = mesh.neighbour();

    // Internal field
    vector Sn;
    vector SfGammaP, SfGammaN;
    scalar SfGammaSnP, SfGammaSnN;
    scalar gammaCorrP, gammaCorrN;
    scalar gammaP, gammaN, invGammaSum;

    forAll(Sf, faceI)
    {
        const label& P = owner[faceI];
        const label& N = neigh[faceI];
        const scalar& beta = weights[faceI];
        const vector& curSf = Sf[faceI];
        Sn = curSf/magSf[faceI];

        // Parameters for owner cell
        SfGammaP = curSf & gamma[P];
        SfGammaSnP = SfGammaP & Sn;
        gammaP = SfGammaSnP/(1-beta);

        gammaCorrP = (SfGammaP - SfGammaSnP*Sn) & gradVf[P];

        // Parameters for neighbour cell
        SfGammaN = curSf & gamma[N];
        SfGammaSnN = SfGammaN & Sn;
        gammaN = SfGammaSnN/beta;

        gammaCorrN = (SfGammaN - SfGammaSnN*Sn) & gradVf[N];

        // Effective parameters
        invGammaSum = 1.0/stabilise(gammaP + gammaN, SMALL);

        gammaMagSf[faceI] = invGammaSum * gammaP * gammaN;
        gammaCorr[faceI] = invGammaSum  * (gammaN*gammaCorrP + gammaP*gammaCorrN);
    }

    // Boundaries
    forAll(mesh.boundaryMesh(), patchI)
    {
        const fvPatchField<scalar>& pVf = vf.boundaryField()[patchI];
        const fvPatchField<vector>& pGradVf = gradVf.boundaryField()[patchI];
        const fvPatchField<tensor>& pGamma = gamma.boundaryField()[patchI];
        const vectorField& pSf = Sf.boundaryField()[patchI];
        const scalarField& pMagSf = magSf.boundaryField()[patchI];
        const scalarField& pBeta = weights.boundaryField()[patchI];

        fvsPatchField<scalar>& pGammaF = gammaMagSf.boundaryField()[patchI];
        fvsPatchField<scalar>& pZetaF = gammaCorr.boundaryField()[patchI];

        tensorField pGammaP = pGamma.patchInternalField();

        if (pVf.coupled())
        {
            tensorField pGammaN = pGamma.patchNeighbourField();

            //TODO - Update fvc::grad implementation to create a coupled gradient field
//             vectorField pGradVfN = pGradVf.patchNeighbourField();
            //TODO - For now we use the patch value for gradVf for both owner and neighbour
            const vectorField& pGradVfP = pGradVf;
            const vectorField& pGradVfN = pGradVf;

            forAll(pSf, faceI)
            {
                const scalar& beta = pBeta[faceI];
                const vector& curSf = pSf[faceI];

                Sn = curSf/pMagSf[faceI];

                // Parameters for owner cell
                SfGammaP = curSf & pGammaP[faceI];
                SfGammaSnP = SfGammaP & Sn;
                gammaP = SfGammaSnP/(1-beta);

                gammaCorrP = (SfGammaP - SfGammaSnP*Sn) & pGradVfP[faceI];

                // Parameters for neighbour cell
                SfGammaN = curSf & pGammaN[faceI];
                SfGammaSnN = SfGammaN & Sn;
                gammaN = SfGammaSnN/beta;

                gammaCorrN = (SfGammaN - SfGammaSnN*Sn) & pGradVfN[faceI];

                // Effective parameters
                invGammaSum = 1.0/stabilise(gammaP + gammaN, SMALL);

                pGammaF[faceI] = invGammaSum * gammaP * gammaN;
                pZetaF[faceI] = invGammaSum  * (gammaN*gammaCorrN + gammaP*gammaCorrP);
            }
        }
        else
        {
            forAll(pSf, faceI)
            {
                const vector& curSf = pSf[faceI];

                Sn = curSf/pMagSf[faceI];

                SfGammaP = curSf & pGammaP[faceI];
                SfGammaSnP = SfGammaP & Sn;
                gammaCorrP = (SfGammaP - SfGammaSnP*Sn) & pGradVf[faceI];

                pGammaF[faceI] = SfGammaSnP;
                pZetaF[faceI] = gammaCorrP;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
