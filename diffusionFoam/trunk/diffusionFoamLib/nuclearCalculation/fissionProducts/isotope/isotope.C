/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        isotope.C
-------------------------------------------------------------------------------------------------*/

#include "isotope.H"
#include "fissionProducts.H"
#include "nuclearField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::isotope::isotope
(
    const fvMesh& mesh,
    const word& name,
    const label& index,
    const dictionary& dict
)
:
    name_(name),
    index_(index),
    lambda_(dict.lookup("lambda")),
    yield_
    (
        IOobject
        (
            word(dict.lookup("yieldName")),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    conc_
    (
        IOobject
        (
            name_,
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless/dimVol, 0.0)
    ),
    sigmaPhi_
    (
        IOobject
        (
            "sigmaPhi_" + name_,
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("sigma*phi", dimless/dimTime, 0.0)
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::isotope::~isotope()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Store the old time values
void Foam::diffusionFoam::isotope::storeOldTime()
{
    conc_.storeOldTime();
    sigmaPhi_.storeOldTime();
}


//- Store the previous iteration values
void Foam::diffusionFoam::isotope::storePrevIter()
{
    conc_.storePrevIter();
    sigmaPhi_.storePrevIter();
}


// ************************************************************************* //
