/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        steadyDiffusionFoam.C
    Application:
        steadyDiffusionFoam
    Description:
        Steady state solver to the multigroup neutron diffusion equation
-------------------------------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "nuclearCalculation.H"
#include "temperatureCalculation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

#   include "setRootCase.H"
#   include "createTime.H"
#   include "createMesh.H"

    // Initialize the nuclear solver object
    autoPtr<Foam::diffusionFoam::nuclearCalculation> nuclCalc;

    nuclCalc.set
    (
        new Foam::diffusionFoam::nuclearCalculation
        (
            mesh,
            false
        )
    );

    // Initialize the temperature solver object
    autoPtr<Foam::diffusionFoam::temperatureCalculation> temCalc;

    temCalc.set
    (
        new Foam::diffusionFoam::temperatureCalculation
        (
            mesh,
            false
        )
    );


    const label maxOuterIters = 1000;
    const scalar keffTolerance = 1E-10;

    scalar keffRes(0), temRes(0);

    Info << "Start of eigenvalue calculation" << endl;

    for (label outerIters=0; outerIters < maxOuterIters; outerIters++)
    {
        Info << "Outer Iterations = " << outerIters << endl;

        // Nuclear Calculation
        if (nuclCalc.valid())
        {
            keffRes = nuclCalc().solve();
            Info << "k-effective residual = " << keffRes << endl;
        }

        // Temperature Calculation
        if (temCalc.valid())
        {
            temRes = temCalc().solve();
            Info << "Temperature residual = " << temRes << endl;
        }

        // Check Eigenvalue convergence
        if (nuclCalc.valid())
        {
            if (keffRes < keffTolerance)
            {
                Info << "Converged!" << endl;
                break;
            }
        }
        else
        {
            break;
        }
    }

    // Write restart data
    runTime.setTime(0,0);
    runTime.writeNow();

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info << "End\n" << endl;

    return(0);
}


// ************************************************************************* //
