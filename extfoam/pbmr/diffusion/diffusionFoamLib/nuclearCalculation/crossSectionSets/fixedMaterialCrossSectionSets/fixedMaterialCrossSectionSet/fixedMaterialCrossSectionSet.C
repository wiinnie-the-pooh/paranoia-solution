/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedMaterialCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#include "fixedMaterialCrossSectionSet.H"

#include "fissionProducts.H"
#include "isotope.H"
#include "volFields.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace crossSections
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fixedMaterialCrossSectionSet::fixedMaterialCrossSectionSet
(
    const label index,
    const label nGroups,
    const label nMaterials,
    IOList<label>& materialID,
    const fissionProducts& products,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    crossSectionSet
    (
        index,
        nGroups,
        products,
        mesh,
        dict,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    materialID_(materialID),
    nMaterials_(nMaterials),
    v_byID_
    (
        IOobject
        (
            word(dict.lookup("meanNeutronVelocity")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
/*
    D_byID_
    (
        IOobject
        (
            word(dict.lookup("diffusionLength")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
*/
    Tr_byID_
    (
        IOobject
        (
            word(dict.lookup("transport")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    A_byID_
    (
        IOobject
        (
            word(dict.lookup("absorption")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    S_byID_(nGroups),
    nuF_byID_
    (
        IOobject
        (
            word(dict.lookup("nuFission")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    F_byID_
    (
        IOobject
        (
            word(dict.lookup("fission")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    chi_byID_
    (
        IOobject
        (
            word(dict.lookup("fissionSpectrum")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    isotopeSigma_byID_(products.nIsotopes()),
    kD_byID_()
{
    // Initialise material-based scattering cross-sections
    const word Sname = dict.lookup("scattering");

    forAll(S_byID_, g)
    {
        if (g != index_)
        {
            S_byID_.set
            (
                g,
                new IOList<scalar>
                (
                    IOobject
                    (
                        Sname + Foam::name(g + 1),
                        mesh.time().constant(),
                        mesh,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE
                    )
                )
            );
        }
    }

    // Material-based isotope specific absorption cross-sections
    forAll (isotopeSigma_byID_, i)
    {
        isotopeSigma_byID_.set
        (
            i,
            new IOList<scalar>
            (
                IOobject
                (
                    dict.lookup(isotope_[i].name()),
                    mesh.time().constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
    }

    // Anisotropic diffusion coefficient correction
    if (dict.found("anisotropicDCorr"))
    {
        if (crossSectionSet::debug)
        {
            Info << "Reading anisotropic diffusion correction vectors" << endl;
        }

        kD_byID_.set
        (
            new IOList<tensor>
            (
                IOobject
                (
                    dict.lookup("anisotropicDCorr"),
                    mesh.time().constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
    }

    // Check each list contains nMaterials values
    checkFields();

    setFields();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

fixedMaterialCrossSectionSet::~fixedMaterialCrossSectionSet()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

#define checkFieldsTest(var,N)                                      \
    if (var.size() != N)                                            \
        FatalErrorIn("fixedMaterialCrossSectionSet::checkFields")   \
            << var.name() << " has length " << var.size()           \
            << ", expected " << N << Foam::abort(FatalError);

//- Check each list contains nMaterials values
void fixedMaterialCrossSectionSet::checkFields()
{
    checkFieldsTest(v_byID_, nMaterials_);
//    checkFieldsTest(D_byID_, nMaterials_);
    checkFieldsTest(Tr_byID_, nMaterials_);
    checkFieldsTest(A_byID_, nMaterials_);
    checkFieldsTest(nuF_byID_, nMaterials_);
    checkFieldsTest(F_byID_, nMaterials_);
    checkFieldsTest(chi_byID_, nMaterials_);

    forAll(S_byID_, g)
    {
        if (g != index_)
        {
            checkFieldsTest(S_byID_[g], nMaterials_);
        }
    }

    forAll(isotopeSigma_byID_, i)
    {
        checkFieldsTest(isotopeSigma_byID_[i], nMaterials_);
    }

    if (kD_byID_.valid())
    {
        checkFieldsTest(kD_byID_(), nMaterials_);
    }
}

#undef checkFieldsTest


//- set the contents of the field variables using the material-based values
void fixedMaterialCrossSectionSet::setFields()
{
    // Repack all cross-sections from material based lists
    //  to volScalarFields

    List<label>::iterator matID(materialID_.begin());
    Field<scalar>::iterator vIter(v_().begin());
//    Field<scalar>::iterator DIter(D_().begin());
    Field<scalar>::iterator TrIter(Tr_().begin());
    Field<scalar>::iterator AIter(A_().begin());
    Field<scalar>::iterator nuFIter(nuF_().begin());
    Field<scalar>::iterator FIter(F_().begin());
    Field<scalar>::iterator chiIter(chi_().begin());

    forAll(materialID_, cellID) // Loop over mesh cells
    {
        *vIter = v_byID_[*matID];
//        *DIter = D_byID_[*matID];
        *TrIter = Tr_byID_[*matID];
        *AIter = A_byID_[*matID];
        *nuFIter = nuF_byID_[*matID];
        *FIter = F_byID_[*matID];
        *chiIter = chi_byID_[*matID];

        matID++;
        vIter++;
//        DIter++;
        TrIter++;
        AIter++;
        nuFIter++;
        FIter++;
        chiIter++;
    }

    // Repack the scattering cross-sections
    forAll(S_, g)
    {
        if (g != index_)
        {
            matID = materialID_.begin();
            Field<scalar>::iterator SIter(S_[g].begin());
            const List<scalar>& curS_byID(S_byID_[g]);

            forAll(materialID_, cellID) // Loop over mesh cells
            {
                *SIter = curS_byID[*matID];

                matID++;
                SIter++;
            }
        }
    }

    // Repack the fission product isotopes microscopic cross-sections
    forAll(isotopeSigma_, i)
    {
        matID = materialID_.begin();
        Field<scalar>::iterator isotopeSigmaIter(isotopeSigma_[i].begin());
        const List<scalar>& curIsotopeSigma_byID(isotopeSigma_byID_[i]);

        forAll(materialID_, cellID) // Loop over mesh cells
        {
            *isotopeSigmaIter = curIsotopeSigma_byID[*matID];

            matID++;
            isotopeSigmaIter++;
        }
    }

    // Repack the anisotropic diffusion coefficient correction, if applicable
    if (kD_byID_.valid())
    {
        crossSectionSet::kD(); // Create kD_ if not already created

        matID = materialID_.begin();
        const IOList<tensor>& kD_byID = kD_byID_();

        Field<tensor>::iterator kDIter(kD_().begin());

        forAll(materialID_, cellID) // Loop over mesh cells
        {
            *kDIter = kD_byID[*matID];
            kDIter++;
        }
    }

    // Now update the removal cross-sections
    updateRemoval();

    // Now make sure the calculated boundaries are up-to-date
    updateBoundaryFields();

    // Now update the total absorption cross-section
    updateIsotopes();
}


//- Update cross-section values.
void fixedMaterialCrossSectionSet::update()
{
    // Make sure the calculated boundaries are up-to-date
    updateBoundaryFields();

    // Update the total absorption cross-section
    updateIsotopes();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
