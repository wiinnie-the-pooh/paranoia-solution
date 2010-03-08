/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        crossSectionSets_.C
-------------------------------------------------------------------------------------------------*/

#include "crossSectionSets.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(crossSectionSets, 0);
defineRunTimeSelectionTable(crossSectionSets, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from mesh and fission product isotope definitions
crossSectionSets::crossSectionSets
(
    const fvMesh& mesh,
    const fissionProducts& products
)
:
    mesh_(mesh),
    group_()
{
}


//- Select given mesh, isotopes and dictionary
autoPtr<crossSectionSets> crossSectionSets::New
(
        const fvMesh& mesh,
        const fissionProducts& products,
        const dictionary& dict
)
{
    word crossSectionSetsName(dict.lookup("type"));

    dictionaryConstructorTable::iterator constructorIter =
        dictionaryConstructorTablePtr_->find(crossSectionSetsName);

    if (constructorIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "autoPtr<crossSectionSets> crossSectionSets::New\n"
            "(\n"
            "    const fvMesh& mesh,\n"
            "    const fissionProducts& products,\n"
            "    const dictionary& dict\n"
            ")",
            dict
        )   << "Unknown cross-section type " << crossSectionSetsName
            << endl << endl
            << "Valid types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalIOError);
    }

    if (debug)
    {
        Info << "Selecting cross-section set " << crossSectionSetsName << endl;
    }

    return autoPtr<crossSectionSets>
    (
        constructorIter()
        (
            mesh,
            products,
            dict
        )
    );
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Check that the total fission spectrum \f$ \sum_g chi_g = 1 \f$
void crossSectionSets::checkChi()
{
    volScalarField sumChi
    (
        IOobject
        (
            "sumChi",
            mesh_.time().constant(),
            mesh_
        ),
        mesh_,
        dimensionedScalar("0", dimless, 0)
    );

    forAll(group_,g)
    {
        sumChi += group_[g].chi();
    }

    forAll(sumChi, cellID)
    {
        if (mag(sumChi[cellID]) != 1)
        {
            WarningIn("crossSectionSets::checkChi")
                << "Total spectrum is not unity in cell " << cellID << "\n";
        }
    }
}


//- Return true if the diffusion coefficient is anisotropic
bool crossSectionSets::anisotropicD() const
{
    forAll(group_, groupI)
    {
        if (group_[groupI].anisotropicD())
        {
            return true;
        }
    }

    return false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

