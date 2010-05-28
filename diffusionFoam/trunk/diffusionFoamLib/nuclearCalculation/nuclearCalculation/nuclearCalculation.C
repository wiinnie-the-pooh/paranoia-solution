/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        nuclearCalculation.C
-------------------------------------------------------------------------------------------------*/

#include "nuclearCalculation.H"
#include "diffusionSolver.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const Foam::diffusionFoam::nuclearCalculation::dictionaryName = "nuclearCalculation";

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::nuclearCalculation::nuclearCalculation
(
    const fvMesh& mesh,
    const Switch& isTransient
)
:
    IOdictionary
    (
        IOobject
        (
            nuclearCalculation::dictionaryName,
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        )
    ),
    maxInnerIter_(readInt(lookup("maxIterations"))),
    solutionTol_(readScalar(lookup("tolerance"))),
    mesh_(mesh),
    products_(mesh_),
    sigma_(
        crossSectionSets::New
        (
            mesh_,
            products_,
            IOdictionary
            (
                IOobject
                (
                    "crossSectionSets",
                    mesh.time().constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        )
    ),
    nuclField_(mesh_, sigma_()),
    dnField_(mesh_),
    isTransient_(isTransient),
    requestedPower_(lookup("requestedPower")),
    powerIsFixed_(lookup("powerIsFixed")),
    timeData_(mesh.time().path()/("timeData"), ios_base::out|ios_base::app)
{
    // Insert the dictionary specified global reactivity
    nuclField_.globalParam().insertReactivity(readScalar(lookup("insertReactivity")));

    // Write title line of time-dependent scalar output file
    timeData_ << "Time \t CPU Time \t Global Power \tk-effective \t omega" << endl;

    isFirstCall_ = true;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::nuclearCalculation::~nuclearCalculation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


//- Inner neutron flux iteration
/*!
    Performs the neutron flux inner iteration, the coupled solution of neutron
    flux and neutron poison isotopic concentrations.
*/
scalar Foam::diffusionFoam::nuclearCalculation::solve()
{
    // Update cross-sections
    sigma_().update();

    // For the first call we must update fission rate and neutron production.
    // This is a workaround for calculated cross-sections, whose values are
    // not set at the time when fission rate and neutron production are
    // first constructed.
    // Todo: Improve... these values should be initialized correctly beforehand
    if (isFirstCall_)
    {
        nuclField_.updateFissionRate();
        nuclField_.updateNeutronProduction();
        nuclField_.updatePower();
        isFirstCall_ = false;
    }

    // Delayed neutrons - update production terms: steady state
    dnField_.updateProductionTerms(isTransient_);

    Info << "Inner neutron flux iteration" << endl;

    scalar residual (0.0);

    for (label iter = 0; iter < maxInnerIter_; iter++)
    {
        storePrevIter();

        // Fission product poisoning
        products_.updateConcentrations(nuclField_, isTransient_);
        nuclField_.updateIsotopes();

        // Diffusion Equation solution
        residual = diffusionSolver
        (
            nuclField_,
            dnField_,
            isTransient_
        ).solve().initialResidual();

        // Update the neutronic fields and global reactor parameters
        nuclField_.updateFissionRate();

        // For fixed power transients & steady-state calculation
        if (!isTransient_ | powerIsFixed_)
        {
            // Update & normalize reactor power
            nuclField_.power().updatePower(nuclField_.F());
            nuclField_.normalizePower(requestedPower_);

            // Update production, buckling & k-effective
            nuclField_.updateNeutronProduction();
            nuclField_.updateLeakage();
            nuclField_.updateKeff(dnField_, isTransient_);
        }

        // No inner iteration for steady-state calculations
        if (!isTransient_)
        {
            residual = mag
            (
                nuclField_.globalParam().keff() 
              - nuclField_.globalParam().keffPrevIter()
            );
            break;
        }

        // Check convergence
        if (residual < solutionTol_)
        {
            // Converged
            Info << "Converged in " << iter << " iterations" << endl;
            break;
        }
    }

    // Update the neutronic fields and global reactor parameters
    // For fixed power transients, these are calculated during the
    // iteration loop so there is no need to repeat this calculation
    if (isTransient_ & !powerIsFixed_)
    {
        // Update reactor power
        nuclField_.power().updatePower(nuclField_.F());

        // Update production, leakage & inverse reactor period
        nuclField_.updateNeutronProduction();
        nuclField_.updateLeakage();
        nuclField_.globalParam().updateReactorPeriod(nuclField_);
    }

    // Updated delayed neutron precursor concentrations: transient
    dnField_.updateConcentrations(nuclField_.P(), isTransient_);

    return residual;
}


//- Store the old time values
void Foam::diffusionFoam::nuclearCalculation::storeOldTime()
{
    nuclField_.storeOldTime();
    dnField_.storeOldTime();
    products_.storeOldTime();
}


//- Store the previous iteration values
void Foam::diffusionFoam::nuclearCalculation::storePrevIter()
{
    nuclField_.storePrevIter();
    dnField_.storePrevIter();
    products_.storePrevIter();
}


//- Write the time-dependent scalar data to file
void Foam::diffusionFoam::nuclearCalculation::writeScalars()
{
    timeData_ << mesh_.time().time().value() << '\t';
    timeData_ << mesh_.time().elapsedCpuTime() << '\t';
    timeData_ << nuclField_.power().globalPower().value() << '\t';
    timeData_ << nuclField_.globalParam().keff() << '\t';
    timeData_ << nuclField_.globalParam().omega().value() << '\t';
    timeData_ << endl;
}


//- Write object
bool Foam::diffusionFoam::nuclearCalculation::writeData(Ostream& os) const
{
    os.writeKeyword("requestedPower") << requestedPower_ << token::END_STATEMENT << nl;
    os.writeKeyword("powerIsFixed") << powerIsFixed_ << token::END_STATEMENT << nl;
    os.writeKeyword("insertReactivity") << 0.0 << token::END_STATEMENT << nl;
    os.writeKeyword("maxIterations") << maxInnerIter_ << token::END_STATEMENT << nl;
    os.writeKeyword("tolerance") << solutionTol_ << token::END_STATEMENT << nl;

    return true;
}

//-----------------------------------------------------------------------------------------------//
