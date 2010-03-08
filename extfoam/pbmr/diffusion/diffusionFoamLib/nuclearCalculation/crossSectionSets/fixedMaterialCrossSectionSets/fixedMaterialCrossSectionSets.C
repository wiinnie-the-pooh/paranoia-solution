/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedMaterialCrossSectionSets.C
-------------------------------------------------------------------------------------------------*/

#include "fixedMaterialCrossSectionSets.H"
#include "fixedMaterialCrossSectionSet.H"
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

defineTypeNameAndDebug(fixedMaterialCrossSectionSets, 0);
addToRunTimeSelectionTable(crossSectionSets, fixedMaterialCrossSectionSets, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fixedMaterialCrossSectionSets::fixedMaterialCrossSectionSets
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
    nMaterials_(max(materialID_)+1)
{
    // Check the materialID list
    if (materialID_.size() != mesh.V().size())
    {
        FatalErrorIn("fixedMaterialCrossSectionSets::fixedMaterialCrossSectionSets")
            << materialID_.name() << " has length " << materialID_.size()
            << ", expected " << mesh.V().size() << Foam::abort(FatalError);
    }

    if (crossSectionSets::debug)
    {
        Info << nMaterials_ << " cross-section sets/materials defined" << endl;
    }

    // Read crossSections dictionary group entries
    PtrList<entry> groupEntries(dict.lookup("groups"));

    group_.setSize(groupEntries.size());

    forAll (groupEntries, g)
    {
        group_.set
        (
            g,
            new fixedMaterialCrossSectionSet
            (
                g,
                groupEntries.size(),
                nMaterials_,
                materialID_,
                products,
                mesh,
                groupEntries[g].dict()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

fixedMaterialCrossSectionSets::~fixedMaterialCrossSectionSets()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update group cross section sets
void fixedMaterialCrossSectionSets::update()
{
    forAll(group_,g)
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
