/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        delayNeutrons.C
-------------------------------------------------------------------------------------------------*/

#include "delayNeutrons.H"
#include "nuclearField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::delayNeutrons::delayNeutrons(const fvMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "delayNeutrons",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    group_(),
    factor1_
    (
        IOobject
        (
            "zeta1",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimless, 0)
    ),
    factor0_
    (
        IOobject
        (
            "zeta0",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimless, 0)
    ),
    P_
    (
        IOobject
        (
            "P_d",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimensionSet(0, -3, -1, 0, 0, 0, 0), 0)
    )
{
    // Read delayNeutronGroups
    PtrList<entry> groupEntries(lookup("groups"));

    group_.setSize(groupEntries.size());

    forAll (group_, k)
    {
        group_.set
        (
            k,
            new delayNeutronGroup
            (
                mesh,
                groupEntries[k].dict()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::delayNeutrons::~delayNeutrons()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the prompt and delayed neutron production terms
/*!
    factor1:
    \f[
        \zeta_1 = 1 + \sum{
                2 \beta_l \frac{1-\frac{1-e^{-\lambda_l\Delta}}{\lambda_l\Delta}}
                {\lambda_l\Delta}
            }
    \f]
    factor0:
    \f[
        \zeta_0 = \sum{
                \beta_l\left( 1+2\frac{e^{-\lambda_l\Delta}
              - \frac{1-e^{-\lambda_l\Delta}}{\lambda_l\Delta}}
                {\lambda_l\Delta} \right) 
            }
    \f]
    P:
    \f[
        Q_d = \sum{
                \lambda_l C_{l0} \left( 1-2\frac{1-e^{-\lambda_l\Delta}}
                {\lambda_l\Delta} \right)
            }
    \f]
    The method addProductionTerms is called for each precursor group.
*/
void Foam::diffusionFoam::delayNeutrons::updateProductionTerms(const bool isTransient)
{
    // Calculate delayed neutron production term factors

    // New prompt production factor
    factor1_.internalField() = scalar(1.0);

    // Old prompt production factor
    factor0_.internalField() = scalar(0.0);

    // Delayed production
    P_.internalField() = scalar(0.0);

    forAll(group_, k)
    {
        group_[k].addProductionTerms(factor1_, factor0_, P_, isTransient);
    }
}


//- Update the delayed neutron precursor concentrations.
//- The method updateConcentrations is called for each precursor group.
void Foam::diffusionFoam::delayNeutrons::updateConcentrations
(
    const volScalarField& P,
    const bool isTransient
)
{
    forAll (group_, k)
    {
        // Warning: this P is passed as a parameter and is not the local member P_
        // IC, 13/02/2008
        group_[k].updateConcentrations(P, isTransient);
    }
}


//- Store the old time values
void Foam::diffusionFoam::delayNeutrons::storeOldTime()
{
    factor1_.storeOldTime();
    factor0_.storeOldTime();
    P_.storeOldTime();

    forAll(group_, k)
    {
        group_[k].storeOldTime();
    }
}


//- Store the previous iteration values
void Foam::diffusionFoam::delayNeutrons::storePrevIter()
{
    factor1_.storePrevIter();
    factor0_.storePrevIter();
    P_.storePrevIter();

    forAll(group_, k)
    {
        group_[k].storePrevIter();
    }
}

// ************************************************************************* //
