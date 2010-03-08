/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polyMaterialCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#include "polyMaterialCrossSectionSets.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace crossSections
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(polyMaterialCrossSectionSets, 0);
addToRunTimeSelectionTable(crossSectionSets, polyMaterialCrossSectionSets, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

polyMaterialCrossSectionSets::polyMaterialCrossSectionSets
(
    const fvMesh& mesh,
    const fissionProducts& products,
    const dictionary& dict
)
:
    crossSectionSets
    (
        mesh,
        products
    ),
    materialID_
    (
        IOobject
        (
            word(dict.lookup("materialID")),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    nMaterials_(max(materialID_)+1),
    refFieldName_(),
    refFieldPtr_(),
    refValue_()
{
    // Check the materialID list
    if (materialID_.size() != mesh.V().size())
    {
        FatalErrorIn("polyMaterialCrossSectionSets::polyMaterialCrossSectionSets")
            << materialID_.name() << " has length " << materialID_.size()
            << ", expected " << mesh.V().size() << Foam::abort(FatalError);
    }

    if (crossSectionSets::debug)
    {
        Info << nMaterials_ << " cross-section sets/materials defined" << endl;
    }

    // Read reference field names
    // Actual field data is read at first call to update
    PtrList<entry> refFieldEntries = dict.lookup("referenceFields");

    refFieldName_.setSize(refFieldEntries.size());
    refFieldPtr_.setSize(refFieldEntries.size());
    refValue_.setSize(refFieldEntries.size());

    forAll(refFieldEntries, fieldI)
    {
        refFieldName_[fieldI] = refFieldEntries[fieldI].keyword();
        refFieldPtr_[fieldI] = 0x0;
        refValue_[fieldI] = List<scalar>(refFieldEntries[fieldI].stream());

        if (refValue_[fieldI].size() != nMaterials_)
        {
            FatalErrorIn("polyMaterialCrossSectionSets::polyMaterialCrossSectionSets")
                << "List " << refFieldName_[fieldI] << " in referenceFields has length "
                << refValue_[fieldI].size() << ", expected " << nMaterials_
                << Foam::abort(FatalError);
        }
    }


    // Read crossSections dictionary group entries
    PtrList<entry> groupEntries(dict.lookup("groups"));

    group_.setSize(groupEntries.size());

    forAll (groupEntries, g)
    {
        group_.set
        (
            g,
            new polyMaterialCrossSectionSet
            (
                g,
                groupEntries.size(),
                nMaterials_,
                materialID_,
                refFieldName_,
                refFieldPtr_,
                refValue_,
                products,
                mesh,
                groupEntries[g].dict()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polyMaterialCrossSectionSets::~polyMaterialCrossSectionSets()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update cross-section values.
void polyMaterialCrossSectionSets::update()
{
    // If not already, find the reference fields in internal database
    forAll(refFieldPtr_, fieldI)
    {
        if (!refFieldPtr_[fieldI])
        {
            refFieldPtr_[fieldI] = &mesh_.lookupObject<volScalarField>
            (
                refFieldName_[fieldI]
            ).internalField();
        }
    }

    forAll(group_, g)
    {
        group_[g].update();
    }

    crossSectionSets::checkChi();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
