/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        decayChain.H
    Class:
        decayChain
    Description:
        A single decay chain declaration. At present this is limited to iodine->Xenon type chains,
        where a parent and daughter isotope are present.
    SourceFiles:
        decayChain.C
-------------------------------------------------------------------------------------------------*/

#ifndef decayChain_H
#define decayChain_H

#include "IOdictionary.H"
#include "isotope.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class nuclearField;

/*---------------------------------------------------------------------------*\
                      Class fissionProducts Declaration
\*---------------------------------------------------------------------------*/
//- A single decay chain declaration. At present this is limited to iodine->Xenon type chains,
//- where a parent and daughter isotope are present.
class decayChain
{
    // Private data
        word name_;

        isotope& parent_;
        isotope& daughter_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        decayChain(const decayChain&);

        //- Disallow default bitwise assignment
        void operator=(const decayChain&);


public:

    // Constructors

        //- Construct from dictionary and fissionProducts
        decayChain
        (
            const word &name,
            fissionProducts& products,
            const dictionary& dict
        );

    // Destructor

        ~decayChain();


    // Member Functions

        // Access

            //- Return a constant reference to the parent isotope
            const isotope& parent() const
            {
                return parent_;
            }

            //- Return a constant reference to the daughter isotope
            const isotope& daughter() const
            {
                return daughter_;
            }

        // Update

            //- Update the chain isotope concentrations for a given fission rate and flux field
            void updateConcentrations
            (
                const volScalarField& F,
                const bool isTransient
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
