/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        nuclearField.C
-------------------------------------------------------------------------------------------------*/

#include "nuclearField.H"
#include "crossSectionSets.H"
#include "delayNeutrons.H"
#include "fissionProducts.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::nuclearField::nuclearField
(
    const fvMesh& mesh,
    crossSectionSets& sigma
)
:
    IOdictionary
    (
        IOobject
        (
            "nuclearField",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    fluxGroups
    (
        mesh,
        lookup("groups"),
        sigma
    ),
    mesh_(mesh),
    globalParam_
    (
        mesh,
        lookup("globalParametersDict")
    ),
    heatProduction_
    (
        mesh,
        lookup("heatProductionDict")
    ),
    F_
    (
        IOobject
        (
            "F",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless/dimVol/dimTime, 0.0)
    ),
    P_
    (
        IOobject
        (
            "P",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless/dimVol/dimTime, 0.0)
    ),
    fixedSrc_(nuclearField::lookup("fixedSource"))
{
    // Update fission rate
    updateFissionRate();

    // Updated neutron production
    updateNeutronProduction();

    // Update power
    heatProduction_.updatePower(F_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::nuclearField::~nuclearField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


//- Update the fission rate fields
void Foam::diffusionFoam::nuclearField::updateFissionRate()
{
    F_ = fissionRate();
}


//- Update the total neutron production fields
void Foam::diffusionFoam::nuclearField::updateNeutronProduction()
{
    P_ = 1/globalParam_.keff()*nProduction();
}


//- Update k-effective
void Foam::diffusionFoam::nuclearField::updateKeff
(
    const delayNeutrons& dnField,
    const bool isTransient
)
{
    globalParam_.updateKEffective(*this, dnField, isTransient);

    P_ *= globalParam_.keffPrevIter()/globalParam_.keff();
}


//- Update reactor power
void Foam::diffusionFoam::nuclearField::updatePower()
{
    heatProduction_.updatePower(F_);
}


//- Normalize the reactor power to the steady state power value
void Foam::diffusionFoam::nuclearField::normalizePower(const dimensionedScalar& newPower)
{
    const scalar factor(newPower.value()/heatProduction_.globalPower().value());

    // Scale power
    F_ *= factor;
    P_ *= factor;

    scaleFlux(factor);
    globalParam_.scale(factor);
    heatProduction_.scale(factor);

    Info << "Global reactor power normalized to " << newPower.value() << endl;
}


//- Store the old time values
void Foam::diffusionFoam::nuclearField::storeOldTime()
{
    globalParam_.storeOldTime();
}


//- Store the previous iteration values
void Foam::diffusionFoam::nuclearField::storePrevIter()
{
    globalParam_.storePrevIter();
}

// ************************************************************************* //
