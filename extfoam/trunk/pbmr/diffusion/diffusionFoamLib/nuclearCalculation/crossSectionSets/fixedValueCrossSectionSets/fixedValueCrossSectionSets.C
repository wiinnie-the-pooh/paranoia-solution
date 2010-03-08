/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        crossSections.C
-------------------------------------------------------------------------------------------------*/

#include "fixedValueCrossSectionSets.H"
#include "fixedValueCrossSectionSet.H"
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

defineTypeNameAndDebug(fixedValueCrossSectionSets, 0);
addToRunTimeSelectionTable(crossSectionSets, fixedValueCrossSectionSets, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fixedValueCrossSectionSets::fixedValueCrossSectionSets
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
    )
{
    // Read crossSections dictionary group entries
    PtrList<entry> groupEntries(dict.lookup("groups"));

    group_.setSize(groupEntries.size());

    forAll (groupEntries, g)
    {
        group_.set
        (
            g,
            new fixedValueCrossSectionSet
            (
                g,
                groupEntries.size(),
                products,
                mesh,
                groupEntries[g].dict()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

fixedValueCrossSectionSets::~fixedValueCrossSectionSets()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update group cross section sets
void fixedValueCrossSectionSets::update()
{
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
