/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        globalNuclearParameters.C
-------------------------------------------------------------------------------------------------*/

#include "globalNuclearParameters.H"
#include "nuclearField.H"
#include "crossSectionSets.H"
#include "delayNeutrons.H"
#include "fissionProducts.H"
#include "diffusionFoamOps.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::globalNuclearParameters::globalNuclearParameters
(
    const fvMesh& mesh,
    const word& dictName    // globalNuclearParameters dictionary name
)
:
    IOdictionary
    (
        IOobject
        (
            dictName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        )
    ),
    mesh_(mesh),
    keffBound_(0.3),
    keff_(readScalar(lookup("keff"))),
    omega_(dimensionedScalar("globalDelayed", dimensionSet(0, 0, -1, 0, 0, 0, 0), 0)),
    globalProd_(dimensionedScalar("globalProd", dimensionSet(0, 0, -1, 0, 0, 0, 0), 0)),
    globalLoss_(dimensionedScalar("globalLoss", dimensionSet(0, 0, -1, 0, 0, 0, 0), 0)),
    globalDelayed_(dimensionedScalar("globalDelayed", dimensionSet(0, 0, -1, 0, 0, 0, 0), 0))
{
    storeOldTime();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::globalNuclearParameters::~globalNuclearParameters()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


//- Update the global reactor multiplication constant k-effective
/*!
    The k-effective value is calculated as:
    \f[
        k = \frac{P_P^{global}}{R_{loss}^{global} + P_d^{global}}
    \f]
    where
    \f[
        P_P^{global} = \sum_j \zeta_1 P_{1,j}^{'''} V_j
    \f]
    \f[
        R_{loss}^{global} = \sum_j \sum_g \left(
                \frac{1}{v_g} \frac{d\phi_g}{dt}
              + \Sigma_{A,g} \phi_g
              - DB^2 \phi_g
            \right) V_j
    \f]
    \f[
        P_d^{global} = \sum_j \left(
                    \zeta_0 \frac{1}{k_0} P_0^{'''}
                    + P_d^{'''} + S_\phi^{'''}
                \right) V_j
    \f]
    where j is the mesh index and g the broad energy group index.
    For steady-state calculations, the time-dependent term is excluded.
*/
void Foam::diffusionFoam::globalNuclearParameters::updateKEffective
(
    const nuclearField& nuclField,
    const delayNeutrons& dnField,
    const bool isTransient
)
{
    storePrevIter();

    const DimensionedField<scalar,volMesh>& vol = nuclField.mesh().V();
    label nGroups = nuclField.nGroups();

    // Global neutron production
    // Note: P_ is stored as 1/k*P so we must remove
    //       the old k when calculating the new k
    globalProd_.value() =
        sum
        (
            keff_ * nuclField.P() * dnField.factor1()
            * vol
        );

    // Global neutron loss
    globalLoss_.value() = 0.0;

    if (nGroups>0)
    {
        for (label g=0; g<nGroups; g++)
        {
            const fluxGroup& curGroup = nuclField.group(g);
            const volScalarField& curPhi = curGroup.phi();
            const crossSectionSet& curSigma = curGroup.sigma();

            if (isTransient)
            {
                globalLoss_.value() +=
                    sum
                    (
                        (
                            curSigma.v()*fvc::ddt(curPhi)
                          + curSigma.Atot()*curPhi
                          - curGroup.leakage()
                        ) * vol
                    );
            }
            else
            {
                globalLoss_.value() +=
                    sum
                    (
                        (
                            curSigma.Atot()*curPhi
                          - curGroup.leakage()
                        ) * vol
                    );
            }
        }
    }

    // Delayed source term
    if (isTransient)
    {
        globalDelayed_.value() =
            sum
            (
                (
                    nuclField.P().oldTime()*dnField.factor0() 
                    - dnField.P()
                    + nuclField.fixedSrc()
                )
                    * vol
            );
        }
    else
    {
        // Delayed source term
        globalDelayed_.value() =
            sum
            (
                (
                    nuclField.P()*dnField.factor0()
                    - dnField.P()
                    + nuclField.fixedSrc()
                )
                    * vol
            );
    }

    // Calculate updated k-effective
    keff_ = (globalProd_/(globalLoss_ - globalDelayed_)).value();

    // Relax
    keff_ = Foam::max(keff_, keffBound_);

    Info<< "k-effective = " << keff_ << endl;
}


//- Update the inverse reactor period
/*!
    The inverse reactor period is calculated as
    \f[
        \omega = \left|\frac{1}{\phi} \frac{d\phi}{dt}\right|
    \f]
    at the location of the maximum change in total flux.
*/
void Foam::diffusionFoam::globalNuclearParameters::updateReactorPeriod
(
    const nuclearField& nuclField
)
{
    storePrevIter();

    volScalarField phi(nuclField.phi());
    volScalarField ddtPhi(nuclField.ddtPhi());

    label maxID = findMaxMag(ddtPhi);

    omega_.value() = ddtPhi[maxID]/phi[maxID];
    omega_.name() = "omega";

    Info<< "Inverse reactor period = " << omega_.value() << endl;
}


//- Scale the global reactor parameters by a given factor
void Foam::diffusionFoam::globalNuclearParameters::scale(const scalar& factor)
{
    globalProd_ *= factor;
    globalLoss_ *= factor;
    globalDelayed_ *= factor;
}


//- Insert an external reactivity
/*!
    \f[
        k \rightarrow \frac{1}{1+\rho}k
    \f]
*/
void Foam::diffusionFoam::globalNuclearParameters::insertReactivity(scalar rho)
{
    keff_ *= 1/(1+rho);
    keffPrevIter_ = keff_;
}


//- Store the old time values
void Foam::diffusionFoam::globalNuclearParameters::storeOldTime()
{
    keff0_ = keff_;

    storePrevIter();
}


//- Store the previous iteration values
void Foam::diffusionFoam::globalNuclearParameters::storePrevIter()
{
    keffPrevIter_ = keff_;
}


//- Write object
bool Foam::diffusionFoam::globalNuclearParameters::writeData(Ostream& os) const
{
    os.writeKeyword("keff") << keff_ << token::END_STATEMENT << nl;
    os.writeKeyword("omega") << omega_ << token::END_STATEMENT << nl;

    return true;
}


// ************************************************************************* //
