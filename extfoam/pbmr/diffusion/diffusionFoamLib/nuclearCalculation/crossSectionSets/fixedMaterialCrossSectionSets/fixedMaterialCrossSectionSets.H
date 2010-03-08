/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedMaterialCrossSectionSets.H
    Class:
        fixedMaterialCrossSectionSets
    Description:
        Fixed value material based broad group cross-sections container class
    SourceFiles:
        fixedMaterialCrossSectionSets.C
-------------------------------------------------------------------------------------------------*/

#ifndef fixedMaterialCrossSectionSets_H
#define fixedMaterialCrossSectionSets_H

#include "crossSectionSets.H"
#include "IOList.H"

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
//- Fixed value material based broad group cross-sections container class
class fixedMaterialCrossSectionSets
:
    public crossSectionSets
{

protected:
    // Private variables
        //- Mesh-based material ID list (Zero-based)
        IOList<label> materialID_;

        //- Number of materials
        label nMaterials_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fixedMaterialCrossSectionSets(const fixedMaterialCrossSectionSets&);

        //- Disallow default bitwise assignment
        void operator=(const fixedMaterialCrossSectionSets&);


public:

    //- Runtime type information
    TypeName("fixedMaterial");

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        fixedMaterialCrossSectionSets
        (
            const fvMesh& mesh,
            const fissionProducts& products,
            const dictionary& dict
        );

    // Destructor
        ~fixedMaterialCrossSectionSets();


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
