/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        crossSection.C
-------------------------------------------------------------------------------------------------*/

#include "crossSectionSet.H"
#include "crossSectionSets.H"

#include "fissionProducts.H"
#include "wallFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(crossSectionSet, 0);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Constructor inline functions

//- Create a new volScalarField, used by crossSectionSet constructor
inline volScalarField* initNewField
(
    const word& name,
    const dictionary& dict,
    const fvMesh& mesh,
    const IOobject::readOption& r,
    const IOobject::writeOption& w,
    const dimensionedScalar& initVal
)
{
    if (r == IOobject::MUST_READ)
    {
        return new volScalarField
        (
            IOobject
            (
                name,
                mesh.time().constant(),
                mesh,
                r,
                w
            ),
            mesh
        );
    }
    else
    {
        return new volScalarField
        (
            IOobject
            (
                name,
                mesh.time().timeName(),
                mesh,
                r,
                w
            ),
            mesh,
            initVal
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

crossSectionSet::crossSectionSet
(
            const label index,
            const label nGroups,
            const fissionProducts& products,
            const fvMesh& mesh,
            const dictionary& dict,
            const IOobject::readOption r,
            const IOobject::writeOption w
)
:
    mesh_(mesh),
    index_(index),
    v_(),
    Tr_(),
    A_(),
    Atot_(),
    S_(nGroups),
    nuF_(),
    F_(),
    chi_(),
    isotopeSigma_(products.nIsotopes()),
    isotope_(products.isotopes())
{

// Field construction

    // Inverse Mean neutron velocity
    v_.set(initNewField(dict.lookup("meanNeutronVelocity"), dict, mesh, r, w,
        dimensionedScalar("1E-6", dimless/dimVelocity, 0)));

    // Transport cross-section
    // Note: Use default 1E20 value to get D near zero
    Tr_.set(initNewField(dict.lookup("transport"), dict, mesh, r, w,
        dimensionedScalar("0", dimless/dimLength, GREAT)));

    // Absorption cross-section
    A_.set(initNewField(dict.lookup("absorption"), dict, mesh, r, w,
        dimensionedScalar("0", dimless/dimLength, 0)));

    // nu-Fission cross-sections
    nuF_.set(initNewField(dict.lookup("nuFission"), dict, mesh, r, w,
        dimensionedScalar("0", dimless/dimLength, 0)));

    // Fission cross-sections
    F_.set(initNewField(dict.lookup("fission"), dict, mesh, r, w,
        dimensionedScalar("0", dimless/dimLength, 0)));

    // Fission spectrum
    chi_.set(initNewField(dict.lookup("fissionSpectrum"), dict, mesh, r, w,
        dimensionedScalar("0", dimless, 0)));

    // Macroscopic scattering cross-sections
    const word Sname = dict.lookup("scattering");

    forAll (S_, g)
    {
        if (g == index_) // Never read or write the removal cross-section
        {
            S_.set
            (
                g,
                initNewField(Sname + Foam::name(g + 1), dict, mesh,
                    IOobject::NO_READ, IOobject::NO_WRITE,
                    dimensionedScalar("0", dimless/dimLength, 0))
            );
        }
        else
        {
            S_.set
            (
                g,
                initNewField(Sname + Foam::name(g + 1), dict, mesh, r, w,
                    dimensionedScalar("0", dimless/dimLength, 0))
            );
        }
    }

    // Isotope specific microscopic neutron absorption cross-sections
    forAll (isotope_, i)
    {
        isotopeSigma_.set
        (
            i,
            initNewField(dict.lookup(isotope_[i].name()), dict, mesh, r, w,
                dimensionedScalar("0", dimLength*dimLength, 0))
        );
    }

    Atot_.set(new volScalarField(A_));

// Field construction complete

    // Now update the removal cross-sections
    updateRemoval();

    // Now make sure the calculated boundaries are up-to-date
    updateBoundaryFields();

    // Now update the total absorption cross-section
    updateIsotopes();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the removal cross-sections
//- \f$ \Sigma_{r,g} = \sum_{g' /notEqual g} \Sigma_S^{g \rightarrow g'} \f$
void crossSectionSet::updateRemoval()
{
    // Total scattering out of group is stored in the main diagonal S_[index_]
    // Initialise S_[index_] as a sum of contributions of all target energy groups
    volScalarField &thisS = S_[index_];

    thisS.internalField() = 0;

    forAll (S_, g)
    {
        if (g != index_)
        {
            thisS += S_[g];
        }
    }
}


//- Update the total absorption cross-sections
//- Include the fission product isotope absorption
//- \f$ \Sigma_a = \Sigma_{a0} + \sum_I \sigma_{a,I} I \f$
void crossSectionSet::updateIsotopes()
{
    Atot_() = A_();

    forAll(isotopeSigma_, i)
    {
        Atot_() += isotopeSigma_[i]*isotope_[i].conc();
    }

    // Update the boundary field
    const fvPatchList& patches = mesh_.boundary();

    forAll(patches, patchI)
    {
        const fvPatch& curPatch = patches[patchI];

        if (isType<wallFvPatch>(curPatch))
        {
            Atot_().boundaryField()[patchI] = Atot_().boundaryField()[patchI].patchInternalField();
        }
    }

}


//- Update the boundary fields
//- Note: update excludes Atot_, this boundary update is done in updateIsotopes
void crossSectionSet::updateBoundaryFields()
{
    const fvPatchList& patches = mesh_.boundary();

    forAll(patches, patchI)
    {
        const fvPatch& curPatch = patches[patchI];

        if (isType<wallFvPatch>(curPatch))
        {
            v_().boundaryField()[patchI] = v_().boundaryField()[patchI].patchInternalField();
            Tr_().boundaryField()[patchI] = Tr_().boundaryField()[patchI].patchInternalField();
            A_().boundaryField()[patchI] = A_().boundaryField()[patchI].patchInternalField();
            nuF_().boundaryField()[patchI] = nuF_().boundaryField()[patchI].patchInternalField();
            F_().boundaryField()[patchI] = F_().boundaryField()[patchI].patchInternalField();
            chi_().boundaryField()[patchI] = chi_().boundaryField()[patchI].patchInternalField();

            forAll(S_, g)
            {
                S_[g].boundaryField()[patchI]
                     = S_[g].boundaryField()[patchI].patchInternalField();
            }

            forAll(isotope_, i)
            {
                isotopeSigma_[i].boundaryField()[patchI]
                     = isotopeSigma_[i].boundaryField()[patchI].patchInternalField();
            }
        }
    }
}


//- Return the anisotropic diffusion correction tensorA()
//- \f$ \vec{k_D} \f$, where \f$ \vec{D}=\vec{k_D} D \f$.
//- create if not available
const volTensorField& crossSectionSet::kD()
{
    if (!kD_.valid())
    {
        kD_.set
        (
            new volTensorField
            (
                IOobject
                (
                    "kD",
                    mesh_.time().constant(),
                    mesh_
                ),
                mesh_,
                dimensionedTensor(word(), dimless, sphericalTensor::one)
            )
        );
    }

    return kD_();
}


//- Return the anisotropic diffusion correction tensorA()
//- \f$ \vec{k_D} \f$, where \f$ \vec{D}=\vec{k_D} D \f$
const volTensorField& crossSectionSet::kD() const
{
    return kD_();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam
} // End namespace Foam

// ************************************************************************* //
