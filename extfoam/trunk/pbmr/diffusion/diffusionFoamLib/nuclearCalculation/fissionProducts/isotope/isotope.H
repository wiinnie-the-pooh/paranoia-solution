/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        isotope.H
    Class:
        isotope
    Description:
        A single isotope, including its name, concentration field and constants
    SourceFiles:
        isotope.C
-------------------------------------------------------------------------------------------------*/

#ifndef isotope_H
#define isotope_H

#include "word.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class fissionProducts;
class nuclearField;

/*---------------------------------------------------------------------------*\
                           Class isotope Declaration
\*---------------------------------------------------------------------------*/
//- A single isotope, including its name, concentration field and constants
class isotope
{
    // Private data

        //- Isotope name
        const word name_;

        //- Isotope ID
        const label index_;

        //- Decay constant \f$ \lambda \f$ [/s]
        dimensionedScalar lambda_;

        //- Fission yield \f$ \gamma \f$
        volScalarField yield_;

        //- Concentration [atom/m^3]
        volScalarField conc_;

        //- Isotope absorption cross-section multiplied by neutron flux
        //- \f$ \sigma\phi \f$
        volScalarField sigmaPhi_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        isotope(const isotope&);

        //- Disallow default bitwise assignment
        void operator=(const isotope&);


public:

    // Constructors

        //- Construct from dictionary
        isotope
        (
            const fvMesh& mesh,
            const word& name,
            const label& index,
            const dictionary& dict
        );


    // Destructor

        ~isotope();


    // Member Functions
        
        // Access

            //- Return the isotope name
            const word& name() const
            {
                return name_;
            }

            //- Return the isotope ID
            const label& index() const
            {
                return index_;
            }

            //- Return a reference to the isotope decay constant \f$ \lambda \f$ [/s]
            const dimensionedScalar& lambda() const
            {
                return lambda_;
            }

            //- Return a reference to the isotope fission yield \f$ \gamma \f$
            const volScalarField& yield() const
            {
                return yield_;
            }

            //- Return a reference to the isotope absorption cross-section multiplied by neutron flux
            //- \f$ \sigma\phi \f$
            volScalarField& sigmaPhi()
            {
                return sigmaPhi_;
            }

            //- Return a reference to the isotope concentration field
            const volScalarField& conc() const
            {
                return conc_;
            }

            //- Return a non-constant reference to the isotope concentration field
            volScalarField& conc()
            {
                return conc_;
            }

        // Update

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#endif

// ************************************************************************* //
