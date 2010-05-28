
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polynomial.H
    Class:
        polynomial
    Description:
        Polynomial expansion class of the form:
            y = A + B1.a + B2.a^2 + .. + C1.b + C2.b^2 + ...
    SourceFiles:
        polynomial.C
-------------------------------------------------------------------------------------------------*/

#ifndef polynomial_H
#define polynomial_H

#include "dictionary.H"
#include "PtrList.H"
#include "Field.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class polynomial Declaration
\*---------------------------------------------------------------------------*/
//- Polynomial expansion class of the form: \n
//- \f$ y = A + B1.a + B2.a^2 + .. + C1.b + C2.b^2 + ... \f$
class polynomial
{
protected:
    // Protected data
        //- Zeroth order value - excludes higher order expansions
        const scalar zeroValue_;

        //- Polynomial coefficients
        List<List<scalar> > coeff_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        polynomial(const polynomial&);

        //- Disallow default bitwise assignment
        void operator=(const polynomial&);

public:

    // Constructors

        //- Construct from dictionary
        polynomial
        (
            const List<word>& refFieldName,
            const dictionary& dict
        );


    // Destructor

        ~polynomial();


    // Member Functions

        //- Evaluate the polynomial for a given set of reference values
        scalar evaluate(const List<scalar>& evaluateValue);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
