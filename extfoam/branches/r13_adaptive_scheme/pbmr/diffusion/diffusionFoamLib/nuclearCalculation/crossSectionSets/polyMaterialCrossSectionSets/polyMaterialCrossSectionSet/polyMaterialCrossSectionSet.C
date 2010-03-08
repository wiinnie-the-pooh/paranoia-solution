/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polyMaterialCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#include "polyMaterialCrossSectionSet.H"
#include "fissionProducts.H"
#include "isotope.H"
#include "diffusionFoamOps.H"
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

polyMaterialCrossSectionSet::polyMaterialCrossSectionSet
(
    const label index,
    const label nGroups,
    const label nMaterials,
    const IOList<label>& materialID,
    const List<word>& refFieldName,
    const List<const Field<scalar>*>& refFieldPtr,
    const List<List<scalar> >& refValue,
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
    refFieldName_(refFieldName),
    refFieldPtr_(refFieldPtr),
    refValue_(refValue),
    v_byID_
    (
        dict.lookup("meanNeutronVelocity"),
        mesh,
        nMaterials,
        refFieldName
    ),
/*
    D_byID_
    (
        dict.lookup("diffusionLength"),
        mesh,
        nMaterials,
        refFieldName
    ),
*/
    Tr_byID_
    (
        dict.lookup("transport"),
        mesh,
        nMaterials,
        refFieldName
    ),
    A_byID_
    (
        dict.lookup("absorption"),
        mesh,
        nMaterials,
        refFieldName
    ),
    S_byID_(nGroups),
    nuF_byID_
    (
        dict.lookup("nuFission"),
        mesh,
        nMaterials,
        refFieldName
    ),
    F_byID_
    (
        dict.lookup("fission"),
        mesh,
        nMaterials,
        refFieldName
    ),
    chi_byID_
    (
        dict.lookup("fissionSpectrum"),
        mesh,
        nMaterials,
        refFieldName
    ),
    isotopeSigma_byID_(products.nIsotopes())
{
    // Scattering cross-sections polynomials
    const word Sname = dict.lookup("scattering");

    forAll(S_byID_, g)
    {
        if (g != index_)
        {
            S_byID_.set
            (
                g,
                new polynomialList
                (
                    Sname + Foam::name(g + 1),
                    mesh,
                    nMaterials,
                    refFieldName
                )
            );
        }
    }

    // Isotope specific absorption cross-sections polynomials
    forAll (isotopeSigma_byID_, i)
    {
        isotopeSigma_byID_.set
        (
            i,
            new polynomialList
            (
                dict.lookup(isotope_[i].name()),
                mesh,
                nMaterials,
                refFieldName
            )
        );
    }

    // Check for anisotropic diffusion coefficient correction
    forAll(Tr_byID_, materialI)
    {
        if (Tr_byID_[materialI].anisotropicCorrected())
        {
            if (crossSectionSet::debug)
            {
                Info << "Diffusion is anisotropic" << endl;
            }

            kD();   // Enable anisotropic correction
            break;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polyMaterialCrossSectionSet::~polyMaterialCrossSectionSet()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


//- set the contents of the field variables using the material-based values
void polyMaterialCrossSectionSet::setFields()
{
    // Iterators to reduce computation time
    List<label>::const_iterator matID(materialID_.begin());
    Field<scalar>::iterator vIter(v_().begin());
//    Field<scalar>::iterator DIter(D_().begin());
    Field<scalar>::iterator TrIter(Tr_().begin());
    Field<scalar>::iterator AIter(A_().begin());
    Field<scalar>::iterator nuFIter(nuF_().begin());
    Field<scalar>::iterator FIter(F_().begin());
    Field<scalar>::iterator chiIter(chi_().begin());

    List<Field<scalar>::iterator> SIter(S_.size());
    forAll(SIter, g)
    {
        SIter[g] = S_[g].begin();
    }

    List<Field<scalar>::iterator> isotopeSigmaIter(isotope_.size());
    forAll(isotopeSigmaIter, i)
    {
        isotopeSigmaIter[i] = isotopeSigma_[i].begin();
    }

    // Reference field values
    List<Field<scalar>::const_iterator> fieldValueIter(refFieldPtr_.size());
    List<scalar> evaluateValue(refFieldPtr_.size());

    forAll(refFieldPtr_, fieldI)
    {
        if (!refFieldPtr_[fieldI])
        {
            FatalErrorIn("polyMaterialCrossSectionSet::setFields")
                << "Reference field " << refFieldName_[fieldI] << " not set."
                << Foam::abort(FatalError);
        }
        fieldValueIter[fieldI] = refFieldPtr_[fieldI]->begin();
    }

    // Perform polynomial evaluation
    forAll(materialID_, cellID) // Loop over mesh cells
    {
        // Evaluate-at values
        // Note: increment iterator inline
        forAll(refFieldPtr_, fieldI)
        {
            evaluateValue[fieldI] = *fieldValueIter[fieldI]++ - refValue_[fieldI][*matID];
        }

        // Do evaluation
        // Note: increment iterators inline
        *vIter++ = v_byID_[*matID].evaluate(evaluateValue);
//        *DIter++ = D_byID_[*matID].evaluate(evaluateValue);
        *TrIter++ = Tr_byID_[*matID].evaluate(evaluateValue);
        *AIter++ = A_byID_[*matID].evaluate(evaluateValue);
        *nuFIter++ = nuF_byID_[*matID].evaluate(evaluateValue);
        *FIter++ = F_byID_[*matID].evaluate(evaluateValue);
        *chiIter++ = chi_byID_[*matID].evaluate(evaluateValue);

        forAll(SIter, g)
        {
            if (g != index_)
            {
                *SIter[g]++ = S_byID_[g][*matID].evaluate(evaluateValue);
            }
        }

        forAll(isotopeSigmaIter, i)
        {
            *isotopeSigmaIter[i]++ = isotopeSigma_byID_[i][*matID].evaluate(evaluateValue);
        }

        matID++;
    }

    // Repack the anisotropic diffusion coefficient correction, if applicable
    // TODO Constant therefore no need to recalculate every time step
    if (kD_.valid())
    {
        matID = materialID_.begin();

        Field<tensor>::iterator kDIter(kD_().begin());

        forAll(materialID_, cellID) // Loop over mesh cells
        {
            *kDIter = Tr_byID_[*matID].anisotropicCorrection();
            kDIter++;
            matID++;
        }
    }
}


//- Update cross-section values
void polyMaterialCrossSectionSet::update()
{
    // Evaluate the cross-sections
    setFields();

    // Now update the removal cross-sections
    updateRemoval();

    // Now make sure the calculated boundaries are up-to-date
    updateBoundaryFields();

    // Now update the total absorption cross-section
    updateIsotopes();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
