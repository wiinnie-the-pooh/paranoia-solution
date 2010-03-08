/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        PolynomialList.C
-------------------------------------------------------------------------------------------------*/

#include "PolynomialList.H"
#include "entry.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
PolynomialList<Type>::PolynomialList
(
    const word& dictName,
    const fvMesh& mesh,
    const label nMaterials,
    const List<word>& refFieldName
)
:
    PtrList<Type>(nMaterials)
{
    IOdictionary dict
    (
        IOobject
        (
            dictName,
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Find polynomial coefficients in dictionary 
    PtrList<entry> dictEntry(dict.lookup("materials"));

    if (dictEntry.size() != nMaterials)
    {
        FatalErrorIn("PolynomialList::PolynomialList")
        << "Subdictionary coefficients in " << dict.name()
        << " contains " << dictEntry.size() << " entries, expected "
        << nMaterials << Foam::abort(FatalError);
    }

    // Construct polynomials
    forAll(dictEntry, entryI)
    {
        const dictionary& curDict = dictEntry[entryI].dict();
        label index = readLabel(curDict.lookup("index"));
        PtrList<Type>::set
        (
            index,
            new Type
            (
                refFieldName,
                curDict
            )
        );
    }

    // Test all materials have been assigned
    forAll(*this, i)
    {
        if (!this->operator()(i))
        {
            FatalIOErrorIn("PolynomialList::PolynomialList", dict)
                << "Not all materials have been assigned";
        }
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
PolynomialList<Type>::~PolynomialList()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
