/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        temperatureCalculation.C
-------------------------------------------------------------------------------------------------*/

#include "temperatureCalculation.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const Foam::diffusionFoam::temperatureCalculation::dictionaryName = "temperatureCalculation";

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::temperatureCalculation::temperatureCalculation
(
    const fvMesh& mesh,
    const Switch& isTransient
)
:
    IOdictionary
    (
        IOobject
        (
            temperatureCalculation::dictionaryName,
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        )
    ),
    mesh_(mesh),
    Tmod_
    (
        IOobject
        (
            word(lookup("moderatorTemperature")),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    Tfuel_
    (
        IOobject
        (
            word(lookup("fuelTemperature")),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    kMod_
    (
        IOobject
        (
            "kMod",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar(lookup("moderatorConductivity"))
    ),
    rhoMod_
    (
        IOobject
        (
            "rhoMod",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar(lookup("moderatorDensity"))
    ),
    cpMod_
    (
        IOobject
        (
            "cpMod",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar(lookup("moderatorSpecificHeat"))
    ),
    puebh_(readScalar(lookup("puebh"))),
    heatSrc_(0x0),
    isTransient_(isTransient)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::temperatureCalculation::~temperatureCalculation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return reference to heatSrc_
const volScalarField& Foam::diffusionFoam::temperatureCalculation::heatSrc() const
{
    if (!heatSrc_)
    {
        heatSrc_ = &mesh_.lookupObject<volScalarField>(lookup("nuclearHeatSource"));
    }

    return *heatSrc_;
}


//- Return reference to Tfuel_
const volScalarField& Foam::diffusionFoam::temperatureCalculation::Tfuel() const
{
    return this->Tfuel_;
}


//- Return reference to Tmod_
const volScalarField& Foam::diffusionFoam::temperatureCalculation::Tmod() const
{
    return this->Tmod_;
}


//- Temperature solution
scalar Foam::diffusionFoam::temperatureCalculation::solve()
{
    // Solve for moderator temperature
    fvMatrix<scalar> tEqn
    (
        - fvm::laplacian(kMod_, Tmod_) == heatSrc()
    );
    if (isTransient_)
    {
        tEqn += fvm::ddt(rhoMod_*cpMod_, Tmod_);
    }

    lduSolverPerformance solverPerf = tEqn.solve();

    // Solve for fuel temperature
    // Todo - fuel kernel overheating model

    Field<scalar>::const_iterator heatSrcIter = heatSrc().begin();
    Field<scalar>::const_iterator TmodIter = Tmod_.begin();
    Field<scalar>::iterator TfuelIter = Tfuel_.begin();

    forAll(Tfuel_, cellI)
    {
        if (*heatSrcIter > 0)
        {
            *TfuelIter++ = (*TmodIter++) + (*heatSrcIter++)*puebh_;
        }
        else
        {
            *TfuelIter++ = 0;
            TmodIter++;
            heatSrcIter++;
        }
    }

    return solverPerf.initialResidual();
}


//-----------------------------------------------------------------------------------------------//
