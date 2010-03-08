/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        tensorPolynomial.C
-------------------------------------------------------------------------------------------------*/

#include "tensorPolynomial.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tensorPolynomial::tensorPolynomial
(
    const List<word>& refFieldName,
    const dictionary& dict
)
:
    polynomial(refFieldName, dict),
    corrected_(false),
    correction_(sphericalTensor::one)
{
    if (dict.found("anisotropicCorr"))
    {
        corrected_ = true;
        correction_ = tensor(dict.lookup("anisotropicCorr"));
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tensorPolynomial::~tensorPolynomial()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
