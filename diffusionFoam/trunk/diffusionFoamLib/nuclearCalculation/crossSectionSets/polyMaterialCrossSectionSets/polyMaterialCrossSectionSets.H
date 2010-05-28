/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polyMaterialCrossSectionSets.H
    Class:
        polyMaterialCrossSectionSets
    Description:
        Polynomial expansion material based broad group cross-sections container class
    SourceFiles:
        polyMaterialCrossSectionSets.C
-------------------------------------------------------------------------------------------------*/

#ifndef polyMaterialCrossSectionSets_H
#define polyMaterialCrossSectionSets_H

#include "polyMaterialCrossSectionSet.H"
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
//- Polynomial expansion material based broad group cross-sections container class
class polyMaterialCrossSectionSets
:
    public crossSectionSets
{

protected:
    // Private variables

        //- Mesh-based material ID list (Zero-based)
        IOList<label> materialID_;

        //- Number of materials
        label nMaterials_;

        //- Names of reference fields... for lookup
        List<word> refFieldName_;

        //- Pointers to reference fields
        List<const Field<scalar>*> refFieldPtr_;

        //- Material-based reference values
        List<List<scalar> > refValue_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        polyMaterialCrossSectionSets(const polyMaterialCrossSectionSets&);

        //- Disallow default bitwise assignment
        void operator=(const polyMaterialCrossSectionSets&);


public:

    //- Runtime type information
    TypeName("polyMaterial");

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        polyMaterialCrossSectionSets
        (
            const fvMesh& mesh,
            const fissionProducts& products,
            const dictionary& dict
        );

    // Destructor
        ~polyMaterialCrossSectionSets();


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
