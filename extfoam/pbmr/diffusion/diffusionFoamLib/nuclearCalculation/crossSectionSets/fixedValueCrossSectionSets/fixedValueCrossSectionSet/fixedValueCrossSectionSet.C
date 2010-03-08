/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedValueCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#include "fixedValueCrossSectionSet.H"

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

fixedValueCrossSectionSet::fixedValueCrossSectionSet
(
    const label index,
    const label nGroups,
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
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
{
    if (dict.found("anisotropicDCorr"))
    {
        if (crossSectionSet::debug)
        {
            Info << "Reading anisotropic diffusion correction vectors" << endl;
        }

        kD_.set
        (
            new volTensorField
            (
                IOobject
                (
                    dict.lookup("anisotropicDCorr"),
                    mesh.time().constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                mesh
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

fixedValueCrossSectionSet::~fixedValueCrossSectionSet()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update cross-section values.
void fixedValueCrossSectionSet::update()
{
    // Update the total absorption cross-section
    updateIsotopes();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
