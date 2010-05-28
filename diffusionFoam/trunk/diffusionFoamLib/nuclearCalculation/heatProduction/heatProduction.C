/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        heatProduction.C
-------------------------------------------------------------------------------------------------*/

#include "heatProduction.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::heatProduction::heatProduction
(
    const fvMesh& mesh,
    const word& dictName
)
:
    dict_
    (
        IOobject
        (
            dictName,
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    Ef_
    (
        IOobject
        (
            dict_.lookup("energyPerFissionName"),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    powerDensity_
    (
        IOobject
        (
            dict_.lookup("powerDensityName"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0)
    ),
    globalPower_(dimensionedScalar("globalPower", dimensionSet(1, 2, -3, 0, 0, 0, 0), 0))
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::heatProduction::~heatProduction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the power density fields and global reactor power
/*!
    Currently, all power is assume to be produced promptly.
    The power density is calculated as:
    \f[
        Q^{'''} = E_f F^{'''}
    \f]
    The global reactor power is calculated as
    \f[
        P_{tot} = \sum Q_j^{'''} V_j
    \f]
    where j is the mesh index.
*/
void Foam::diffusionFoam::heatProduction::updatePower(const volScalarField& F)
{
    powerDensity_ = Ef_*F;

    globalPower_ = sum(powerDensity_*F.mesh().V());

    Info << "Global reactor power = " << globalPower_.value() << endl;
}


//- Scale the nuclear power
void Foam::diffusionFoam::heatProduction::scale(const scalar& factor)
{
    powerDensity_ *= factor;
    globalPower_ *= factor;
}


//- Store the old time values
void Foam::diffusionFoam::heatProduction::storeOldTime()
{
}


//- Store the previous iteration values
void Foam::diffusionFoam::heatProduction::storePrevIter()
{
}


//- Write object
bool Foam::diffusionFoam::heatProduction::writeData(Ostream& os) const
{
    os.writeKeyword("golbalPower") << globalPower_ << token::END_STATEMENT << nl;
    return true;
}

//-----------------------------------------------------------------------------------------------//
