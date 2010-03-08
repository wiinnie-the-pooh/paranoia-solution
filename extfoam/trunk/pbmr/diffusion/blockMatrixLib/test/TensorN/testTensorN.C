/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        test.C
    Application:
        test
    Description:
        Test tensorN inversions
-------------------------------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "blockFvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

#   include "setRootCase.H"
#   include "createTime.H"
#   include "createMesh.H"


    Foam::tensor8 tmpTensor;
    tmpTensor[0] = 0.04495578024;
    tmpTensor[1] = -0.00004090193309;
    tmpTensor[2] = -0.0002481146828;
    tmpTensor[3] = -0.0009685577755;
    tmpTensor[4] = -0.0007400760206;
    tmpTensor[5] = -0.005629528668;
    tmpTensor[6] = -0.006743483924;
    tmpTensor[7] = -0.01065299739;
    tmpTensor[8] = -0.04139114899;
    tmpTensor[9] = 0.01760428565;
    tmpTensor[10] = -0.0001847146686;
    tmpTensor[11] = -0.0007210650594;
    tmpTensor[12] = -0.0005509665745;
    tmpTensor[13] = -0.004191031786;
    tmpTensor[14] = -0.005020341336;
    tmpTensor[15] = -0.007930868339;
    tmpTensor[16] = -0.000001453702337;
    tmpTensor[17] = -0.0157641406;
    tmpTensor[18] = 0.01628704016;
    tmpTensor[19] = -0.0000004394161853;
    tmpTensor[20] = -0.0000003357583719;
    tmpTensor[21] = -0.000002554009761;
    tmpTensor[22] = -0.000003059390009;
    tmpTensor[23] = -0.000004833061685;
    tmpTensor[24] = 0;
    tmpTensor[25] = 0;
    tmpTensor[26] = -0.01360149675;
    tmpTensor[27] = 0.03315003564;
    tmpTensor[28] = 0;
    tmpTensor[29] = 0;
    tmpTensor[30] = 0;
    tmpTensor[31] = 0;
    tmpTensor[32] = 0;
    tmpTensor[33] = 0;
    tmpTensor[34] = 0;
    tmpTensor[35] = -0.02418419689;
    tmpTensor[36] = 0.07610986336;
    tmpTensor[37] = -0.007517395466;
    tmpTensor[38] = -0.00000633989567;
    tmpTensor[39] = 0;
    tmpTensor[40] = 0;
    tmpTensor[41] = 0;
    tmpTensor[42] = 0;
    tmpTensor[43] = 0;
    tmpTensor[44] = -0.06845142441;
    tmpTensor[45] = 0.1019344618;
    tmpTensor[46] = -0.05035555681;
    tmpTensor[47] = -0.004232234455;
    tmpTensor[48] = 0;
    tmpTensor[49] = 0;
    tmpTensor[50] = 0;
    tmpTensor[51] = 0;
    tmpTensor[52] = -0.0001861168439;
    tmpTensor[53] = -0.0853634502;
    tmpTensor[54] = 0.09011193634;
    tmpTensor[55] = -0.1145578347;
    tmpTensor[56] = 0;
    tmpTensor[57] = 0;
    tmpTensor[58] = 0;
    tmpTensor[59] = 0;
    tmpTensor[60] = 0;
    tmpTensor[61] = -0.001970039605;
    tmpTensor[62] = -0.03149864804;
    tmpTensor[63] = 0.130988998;

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;
/*
    for (label i=0; i<4000000; i++)
    {
        inv(tmpTensor);
    }
*/
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info << inv(tmpTensor) << endl;

    Info << "End\n" << endl;

    return(0);
}


// ************************************************************************* //
