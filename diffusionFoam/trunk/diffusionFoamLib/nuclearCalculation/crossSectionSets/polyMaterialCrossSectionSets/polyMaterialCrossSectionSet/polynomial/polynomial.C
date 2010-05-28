/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polynomial.C
-------------------------------------------------------------------------------------------------*/

#include "polynomial.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

polynomial::polynomial
(
    const List<word>& refFieldName,
    const dictionary& dict
)
:
    zeroValue_(readScalar(dict.lookup("zeroValue"))),
    coeff_(refFieldName.size())
{
    forAll(refFieldName, fieldI)
    {
        if (dict.found(refFieldName[fieldI]))
        {
            coeff_[fieldI] = Field<scalar>(dict.lookup(refFieldName[fieldI]));
        }
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polynomial::~polynomial()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Evaluate the polynomial
scalar polynomial::evaluate(const List<scalar>& evaluateValue)
{
    scalar retValue = zeroValue_;

    forAll(coeff_, i)
    {
        scalar curValue = 0;
        const scalar& curEvaluateValue = evaluateValue[i];

        forAll(coeff_[i], j)
        {
            curValue += coeff_[i][j];
            curValue *= curEvaluateValue;
        }

        retValue += curValue;
    }

    return retValue;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
