/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedValueCrossSectionSets.H
    Class:
        fixedValueCrossSectionSets
    Description:
        Fixed value broad group cross-sections container class
    SourceFiles:
        fixedValueCrossSectionSets.C
-------------------------------------------------------------------------------------------------*/

#ifndef fixedValueCrossSectionSets_H
#define fixedValueCrossSectionSets_H

#include "crossSectionSets.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class Forward Declarations
class fissionProducts;
class isotope;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace crossSections
{

/*---------------------------------------------------------------------------*\
                         Class crossSections Declaration
\*---------------------------------------------------------------------------*/
//- Fixed value broad group cross-sections container class
class fixedValueCrossSectionSets
:
    public crossSectionSets
{

protected:
    // Private Member Functions

        //- Disallow default bitwise copy construct
        fixedValueCrossSectionSets(const fixedValueCrossSectionSets&);

        //- Disallow default bitwise assignment
        void operator=(const fixedValueCrossSectionSets&);


public:

    //- Runtime type information
    TypeName("fixedValue");

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        fixedValueCrossSectionSets
        (
            const fvMesh& mesh,
            const fissionProducts& products,
            const dictionary& dict
        );

    // Destructor
        ~fixedValueCrossSectionSets();


    // Member Functions

        // Update state

            //- Update group cross section sets
            virtual void update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
