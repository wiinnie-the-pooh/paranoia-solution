
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedValueCrossSectionSet.H
    Class:
        fixedValueCrossSectionSet
    Description:
        Container class for macroscopic nuclear broad-group cross-sections and other broad-group
        nuclear data for a single broad energy group.
    SourceFiles:
        fixedValueCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#ifndef fixedValueCrossSectionSet_H
#define fixedValueCrossSectionSet_H

#include "crossSectionSet.H"

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
                         Class crossSection Declaration
\*---------------------------------------------------------------------------*/
//- Container class for macroscopic nuclear broad-group cross-sections and
//- other broad-group nuclear data for a single broad energy group.
class fixedValueCrossSectionSet
:
    public crossSectionSet
{
protected:
    // Private Member Functions

        //- Disallow default bitwise copy construct
        fixedValueCrossSectionSet(const fixedValueCrossSectionSet&);

        //- Disallow default bitwise assignment
        void operator=(const fixedValueCrossSectionSet&);


public:

    // Constructors

        //- Construct from dictionary
        fixedValueCrossSectionSet
        (
            const label index,
            const label nGroups,
            const fissionProducts& products,
            const fvMesh& mesh,
            const dictionary& dict
        );


    // Destructor

        ~fixedValueCrossSectionSet();


    // Member Functions

        // Update

            //- Update crossSection values
            virtual void update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
