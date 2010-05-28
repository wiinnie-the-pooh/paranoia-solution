/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        diffusionFoamOps.C
-------------------------------------------------------------------------------------------------*/

#include "diffusionFoamOps.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Modification of Foam::findMax to find maximum absolute element of a Field
label findMaxMag(const scalarField& l)
{
    label index = 0;
    scalar maxVal = 0.0;
    scalar curVal = 0.0;

    forAll(l, i)
    {
        curVal = mag(l[i]);
        if (curVal > maxVal)
        {
            index = i;
            maxVal = curVal;
        }
    }

    return index;
}


//- Locate entry names in the specified object registry
template<class T>
List<const T*> findEntriesInDB
(
    const PtrList<entry>& entries,
    const objectRegistry& db
)
{
    List<const T*> fields(entries.size());

    forAll(entries, fieldI)
    {
        const T& curField = db.lookupObject<T>
        (
            entries[fieldI].keyword()
        );

        fields[fieldI] = &curField;
    }

    return fields;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
