/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fissionProducts.C
-------------------------------------------------------------------------------------------------*/

#include "fissionProducts.H"
#include "nuclearField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::fissionProducts::fissionProducts(const fvMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "fissionProducts",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    isotope_(),
    decayChain_()
{
    // Read isotopes
    Info << "Reading isotope information" << endl;
    PtrList<entry> isotopeEntries(lookup("isotopes"));

    isotope_.setSize(isotopeEntries.size());

    forAll (isotope_, i)
    {
        isotope_.set
        (
            i,
            new isotope
            (
                mesh,
                isotopeEntries[i].keyword(),
                i,
                isotopeEntries[i].dict()
            )
        );
    }


    // Read decay chains
    Info << "Reading decay chain information" << endl;
    PtrList<entry> decayChainEntries(lookup("decayChains"));

    decayChain_.setSize(decayChainEntries.size());

    forAll (decayChain_, j)
    {
        decayChain_.set
        (
            j,
            new decayChain
            (
                decayChainEntries[j].keyword(),
                *this,
                decayChainEntries[j].dict()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::fissionProducts::~fissionProducts()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the fission product concentrations for all decay chains
/*!
    The method updateConcentrations is called for each decay chain using the
    updated neutron flux and microscopic absorption cross-sections for each
    isotope.
*/
void Foam::diffusionFoam::fissionProducts::updateConcentrations
(
    const nuclearField& nuclField,
    bool isTransient
)
{
    storePrevIter();

    forAll(isotope_, i)
    {
        isotope_[i].sigmaPhi() = nuclField.isotopeSigmaPhi(i);
    }

    forAll(decayChain_, j)
    {
        decayChain_[j].updateConcentrations(nuclField.F(), isTransient);
    }
}


//- Return the constant reference to an isotope by name

Foam::diffusionFoam::isotope& Foam::diffusionFoam::fissionProducts::isotopeByName(const word name)
{
    label ID = -1;

    forAll(isotope_, i)
    {
        if (isotope_[i].name() == name)
        {
            ID = i;
        }
    }

    if (ID == -1)
    {
        FatalErrorIn("Foam::diffusionFoam::fissionProducts::isotopeByName") <<
                "Isotope " << name << " not found" <<
                Foam::abort(FatalError);
    }

    return isotope_[ID];
}


//- Store the old time values
void Foam::diffusionFoam::fissionProducts::storeOldTime()
{
    forAll(isotope_,i)
    {
        isotope_[i].storeOldTime();
    }
}


//- Store the previous iteration values
void Foam::diffusionFoam::fissionProducts::storePrevIter()
{
    forAll(isotope_,i)
    {
        isotope_[i].storePrevIter();
    }
}


// ************************************************************************* //
