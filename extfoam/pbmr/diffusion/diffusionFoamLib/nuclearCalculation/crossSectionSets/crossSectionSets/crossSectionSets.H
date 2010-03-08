/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        crossSectionSets.H
    Class:
        crossSectionSets
    Description:
        Virtual base class defining broad group material cross-sections sets
    SourceFiles:
        crossSectionSets.C
-------------------------------------------------------------------------------------------------*/

#ifndef crossSectionSets_H
#define crossSectionSets_H

#include "crossSectionSet.H"
#include "fissionProducts.H"
#include "fvMesh.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

/*---------------------------------------------------------------------------*\
                         Class crossSections Declaration
\*---------------------------------------------------------------------------*/
//- Virtual base class defining broad group material cross-sections sets
class crossSectionSets
{
protected:
    // Protected data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- List of delay neutron groups
        PtrList<crossSectionSet> group_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        crossSectionSets(const crossSectionSets&);

        //- Disallow default bitwise assignment
        void operator=(const crossSectionSets&);

public:

    //- Runtime type information
    TypeName("crossSectionSets");

#ifndef SWIG
    // Declare run-time constructor selection tables
        declareRunTimeSelectionTable
        (
            autoPtr,
            crossSectionSets,
            dictionary,
            (
                const fvMesh& mesh,
                const fissionProducts& products,
                const dictionary& dict
            ),
            (
                mesh,
                products,
                dict
            )
        );
#endif

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        crossSectionSets
        (
            const fvMesh& mesh,
            const fissionProducts& products
        );

    // Selectors

        //- Select given mesh, isotopes and dictionary
        static autoPtr<crossSectionSets> New
        (
                const fvMesh& mesh,
                const fissionProducts& products,
                const dictionary& dict
        );

    // Destructor
        virtual ~crossSectionSets()
        {}


    // Member Functions

        // Access

            //- Return the number of energy groups
            label nGroups()
            {
                return group_.size();
            }

            //- Return the cross-section sets for a given energy group
            crossSectionSet& group(const label i)
            {
                return group_[i];
            }

            //- Return true if the diffusion coefficient is anisotropic
            bool anisotropicD() const;


        // Test

            //- Check that the total fission spectrum \f$ \sum_g chi_g = 1 \f$
            void checkChi();

        // Update state

            //- Update group cross section sets
            virtual void update()
            {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
