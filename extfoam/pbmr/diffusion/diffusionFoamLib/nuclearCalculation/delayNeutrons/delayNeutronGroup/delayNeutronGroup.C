/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        delayNeutronGroup.C
-------------------------------------------------------------------------------------------------*/

#include "delayNeutronGroup.H"
#include "delayNeutrons.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::delayNeutronGroup::delayNeutronGroup
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    lambda_(dict.lookup("lambda")),
    beta_
    (
        IOobject
        (
            word(dict.lookup("betaName")),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    lambdaC_
    (
        IOobject
        (
            word(dict.lookup("lambdaCName")),
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimensionSet(0, -3, -1, 0, 0, 0, 0), 0)
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::delayNeutronGroup::~delayNeutronGroup()
{}


//- Update the delayed neutron centrations
/*!
    Updated delayed neutron precursor group concentrations are calculated
    based on the supplied neutron production (total).
    For the time-dependent case:
    \f[
        \lambda_l C_l^1 = \lambda_l C_l^0 e^{-\lambda_l \Delta}
              + \beta_l P_0^{'''} \left(
                    \frac{1-e^{-\lambda_l \Delta}}{\lambda_l \Delta}
                  - e^{-\lambda_l \Delta}
                \right)
              + \beta_l P_1^{'''} \left(
                    1 - \frac{1-e^{-\lambda_l \Delta}}{\lambda_l \Delta}
                \right)
    \f]
    For the steady-state case:
    \f[
        \lambda_l C_l = \beta_l P^{'''}
    \f]
*/
void Foam::diffusionFoam::delayNeutronGroup::updateConcentrations
(
    const volScalarField& P,
    const bool isTransient
)
{
    storePrevIter();

    if (isTransient)
    {
        // Transient case
        dimensionedScalar deltaT (P.mesh().time().deltaT());

        // Lambda*deltaT temporary buffer for production/decay
        // of delayed neutrons
        dimensionedScalar lambdaT = lambda_*deltaT;

        // e^(-lambda*deltaT) temporary buffer for production/decay
        // of delayed neutrons
        dimensionedScalar expLambdaT = Foam::exp(-lambdaT);

        lambdaC_ =
            (lambdaC_.oldTime() - beta_*P.oldTime())*expLambdaT
            - beta_*(P - P.oldTime())*(scalar(1) - expLambdaT)/lambdaT
            + beta_*(P);
    }
    else
    {
        // Steady state case
        lambdaC_ = beta_*P;
    }
}


//- Include this delayed neutron group's contribution to the diffusion terms
/*!
    factor1:
    \f[
        \zeta_1 += 2\beta_l \frac{1-\frac{1-e^{-\lambda_l\Delta}}{\lambda_l\Delta}}
                    {\lambda_l\Delta}
    \f]
    factor0:
    \f[
        \zeta_0 += \beta_l\left( 1+2\frac{e^{-\lambda_l\Delta}
                 -\frac{1-e^{-\lambda_l\Delta}}{\lambda_l\Delta}}s
                  {\lambda_l\Delta} \right) 
    \f]
    P:
    \f[
        Q_d += \lambda_l C_{l0} \left( 1-2\frac{1-e^{-\lambda_l\Delta}}
               {\lambda_l\Delta} \right)
    \f]
*/
void Foam::diffusionFoam::delayNeutronGroup::addProductionTerms
(
    volScalarField& factor1,
    volScalarField& factor0,
    volScalarField& P,
    const bool isTransient
)
{
    if (isTransient)
    {
        // Transient case

        dimensionedScalar lambdaT(lambda_ * P.mesh().time().deltaT());
        dimensionedScalar expLambdaT(Foam::exp(-lambdaT));
        dimensionedScalar expTerm((1.0 - expLambdaT)/lambdaT);

        factor1 -=
            beta_* 2.0 * ((1.0 - expTerm)/lambdaT);

        factor0 +=
            beta_
          * (1.0 + 2.0 * (expLambdaT - expTerm)/lambdaT);

        P +=
            lambdaC_.oldTime()
          * (1.0 - 2.0 * expTerm);
    }
    else
    {
        // Steady state case

        factor0 += beta_;

        P += lambdaC_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Store the old time values
void Foam::diffusionFoam::delayNeutronGroup::storeOldTime()
{
    lambdaC_.storeOldTime();
}


//- Store the previous iteration values
void Foam::diffusionFoam::delayNeutronGroup::storePrevIter()
{
    lambdaC_.storePrevIter();
}


// ************************************************************************* //
