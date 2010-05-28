/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        decayChain.C
-------------------------------------------------------------------------------------------------*/

#include "decayChain.H"
#include "fissionProducts.H"
#include "nuclearField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::decayChain::decayChain
(
    const word &name,
    fissionProducts& products,
    const dictionary& dict
)
:
    name_(name),
    parent_(products.isotopeByName(dict.lookup("parent"))),
    daughter_(products.isotopeByName(dict.lookup("daughter")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::decayChain::~decayChain()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the chain isotope concentrations for a given fission rate and flux field.
/*! The current implementation assumes a parent and daughter isotope, such as in the
    case of the Iodine->Xenon decay chain. \n
    For the time-dependent case: \n
    \f[
        I_1 = I_0 e^{-\lambda_I \Delta} + \frac{\gamma_I}{\lambda_I} \bar{F}
            \left( 1-e^{-\lambda_I \Delta} \right)
    \f]
    \f[
        X_1 = X_0 e^{-\lambda_2 \Delta}
            + \left( \gamma_X + \gamma_I \right) \bar{F}
            \frac{1-e^{-\lambda_2 \Delta}}{\lambda_2}
            + \left( \lambda_I I_0 -\gamma_I \bar{F} \right)
            \frac{e^{-\lambda_I \Delta} - e^{-\lambda_2 \Delta}}{\lambda_2-\lambda_I}
    \f]
    For the steady-state case: \n
    \f[
        I = \frac{\gamma_I}{\lambda_I} F
    \f]
    \f[
        X = \frac{\gamma_X + \gamma_I}{\lambda_2} F
    \f]
    where \f$ \lambda_2 = \lambda_X + \sigma_X \phi \f$
*/
void Foam::diffusionFoam::decayChain::updateConcentrations
(
    const volScalarField& F,
    const bool isTransient
)
{
    const dimensionedScalar& lambdaI = parent_.lambda();
    const dimensionedScalar& lambdaX = daughter_.lambda();

    const volScalarField& yieldI = parent_.yield();
    const volScalarField& yieldX = daughter_.yield();

    volScalarField& concI = parent_.conc();
    volScalarField& concX = daughter_.conc();

    if (isTransient)
    {
// Time-dependent case
        const dimensionedScalar& deltaT = concI.mesh().time().deltaT();

        // Mean fission rate for time interval
        // Don't confuse F_ with F
        volScalarField F_(0.5*(F.oldTime() + F));

        // Mean neutron flux for time interval
        // Don't confuse sigmaPhiX_ with sigmaPhiX
        volScalarField sigmaPhiX_(0.5*(daughter_.sigmaPhi() + daughter_.sigmaPhi().oldTime()));

        volScalarField lambda2 (lambdaX + sigmaPhiX_);

        dimensionedScalar expLambdaTI = Foam::exp(-lambdaI*deltaT);
        volScalarField expLambdaT2 = Foam::exp(-lambda2*deltaT);

        // Daughter isotope
        concX =
            concX.oldTime()*expLambdaT2
            + (yieldX+yieldI)*F_*(1.0 - expLambdaT2)/lambda2
            + (lambdaI*concI.oldTime() - yieldI*F_)
                * (expLambdaTI - expLambdaT2)
                / (lambda2 - lambdaI);

        // Parent isotope
        concI =
            concI.oldTime()*expLambdaTI
            + (1.0-expLambdaTI) * yieldI * F_
            / lambdaI;
    }
    else
    {
// Steady state case
        // Daughter isotope
        concX = (yieldX + yieldI)*F/(lambdaX + daughter_.sigmaPhi());

        // Parent isotope
        concI = yieldI/lambdaI*F;
    }

}


// ************************************************************************* //
