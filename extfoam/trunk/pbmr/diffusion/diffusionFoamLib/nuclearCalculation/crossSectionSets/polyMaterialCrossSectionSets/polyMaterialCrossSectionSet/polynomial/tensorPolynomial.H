
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        tensorPolynomial.H
    Class:
        tensorPolynomial
    Description:
        Polynomial expansion class of the form:
            y = A + B1.a + B2.a^2 + .. + C1.b + C2.b^2 + ...
        Includes an anisotropic tensor correction factor
    SourceFiles:
        tensorPolynomial.C
-------------------------------------------------------------------------------------------------*/

#ifndef tensorPolynomial_H
#define tensorPolynomial_H

#include "polynomial.H"
#include "tensor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class tensorPolynomial Declaration
\*---------------------------------------------------------------------------*/
//- Polynomial expansion class of the form: \n
//- \f$ y = A + B1.a + B2.a^2 + .. + C1.b + C2.b^2 + ... \f$
//- Includes an anisotropic tensor correction factor
class tensorPolynomial
:
    public polynomial
{
protected:
    // Protected data
        //- True if a tensor correction is applicable
        bool corrected_;

        //- Correction tensor
        tensor correction_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        tensorPolynomial(const tensorPolynomial&);

        //- Disallow default bitwise assignment
        void operator=(const tensorPolynomial&);

public:

    // Constructors

        //- Construct from dictionary
        tensorPolynomial
        (
            const List<word>& refFieldName,
            const dictionary& dict
        );


    // Destructor

        ~tensorPolynomial();


    // Member Functions

        //- Returns true if an anisotropic tensor correction is applicable
        inline bool anisotropicCorrected() const
        {
            return corrected_;
        }

        //- Return the anisotropic correction vector
        inline const tensor& anisotropicCorrection() const
        {
            return correction_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
