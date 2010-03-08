/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        delayNeutronGroup.H
    Class:
        delayNeutronGroup
    Description:
        Class definition for a single delayed neutron precursor group
    SourceFiles:
        delayNeutronGroup.C
-------------------------------------------------------------------------------------------------*/

#ifndef delayNeutronGroup_H
#define delayNeutronGroup_H

#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class delayNeutrons;

/*---------------------------------------------------------------------------*\
                       Class delayNeutronGroup Declaration
\*---------------------------------------------------------------------------*/
//- Class definition for a single delayed neutron precursor group
class delayNeutronGroup
{
    // Private data

        //- Group decay constant \f$ \lambda_l \f$
        dimensionedScalar lambda_;

        //- Group yield per fission field \f$ \beta_l \f$
        volScalarField beta_;

        //- Group precursor concentration field \f$ \lambda_l C_l \f$
        volScalarField lambdaC_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        delayNeutronGroup(const delayNeutronGroup&);

        //- Disallow default bitwise assignment
        void operator=(const delayNeutronGroup&);


public:

    // Constructors

        //- Construct from dictionary
        delayNeutronGroup
        (
            const fvMesh& mesh,
            const dictionary& dict
        );


    // Destructor

        ~delayNeutronGroup();


    // Member Functions

        // Access

            //- Return the group decay constant \f$ \lambda_l \f$
            const dimensionedScalar& lambda() const
            {
                return lambda_;
            }

            //- Return the precursor group yield per fission \f$ \beta_l \f$
            const volScalarField& beta() const
            {
                return beta_;
            }

            //- Return the precursor concentration field \f$ \lambda_l C_l \f$
            const volScalarField& lambdaC() const
            {
                return lambdaC_;
            }

            //- Include this delayed neutron group's contribution to the prompt and delayed source terms
            void addProductionTerms
            (
                volScalarField& factor1,
                volScalarField& factor0,
                volScalarField& P,
                const bool isTransient
            );

        // Update

            //- Update the delayed neutron concentrations \f$ \lambda_l C_l \f$
            void updateConcentrations
            (
                const volScalarField& P,
                const bool isTransient
            );

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
