/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        transientDiffusionFoam.C
    Application:
        transientDiffusionFoam
    Description:
        Time-dependent solver to the multigroup neutron diffusion equation
-------------------------------------------------------------------------------------------------*/

#include "nuclearCalculation.H"
#include "temperatureCalculation.H"
#include "fvCFD.H"

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
            true
        )
    );

    // Initialize the temperature solver object
    autoPtr<Foam::diffusionFoam::temperatureCalculation> temCalc;

    temCalc.set
    (
        new Foam::diffusionFoam::temperatureCalculation
        (
            mesh,
            true
        )
    );

    scalar nuclRes(0), temRes(0);

    Info << "Start of time-dependent calculation" << endl;

    for (runTime++; !runTime.end(); runTime++)
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        // Nuclear Calculation
        if (nuclCalc.valid())
        {
            nuclCalc().storeOldTime();

            nuclRes = nuclCalc().solve();

            // Write time-dependent scalar data to file
            nuclCalc().writeScalars();
        }

        // Temperature Calculation
        if (temCalc.valid())
        {
            temRes = temCalc().solve();
        }

        // Write restart data
        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info << "End\n" << endl;

    return(0);
}


// ************************************************************************* //
