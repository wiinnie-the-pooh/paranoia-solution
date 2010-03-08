/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        diffusionFoamOps.H
    Namespace:
        diffusionFoamOps
    Description:
        Generic utility functions and subroutines
    SourceFiles:
        diffusionFoamOps.C
-------------------------------------------------------------------------------------------------*/

#ifndef diffusionFoamOps_H
#define diffusionFoamOps_H

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    //- Modification of Foam::findMax to find maximum absolute element of a volScalarField
    label findMaxMag(const scalarField& l);

    //- Locate entry names in the specified object registry
    template<class T>
    List<T*> findEntriesInDB
    (
        const PtrList<entry>& entries,
        const objectRegistry& db
    );
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
