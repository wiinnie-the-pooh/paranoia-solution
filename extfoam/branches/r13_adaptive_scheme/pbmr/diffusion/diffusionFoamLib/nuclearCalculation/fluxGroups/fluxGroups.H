/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fluxGroups.H
    Class:
        fluxGroups
    Description:
        Scalar neutron flux fields for multiple broad group fluxes.
    SourceFiles:
        fluxGroups.C
-------------------------------------------------------------------------------------------------*/

#ifndef fluxGroups_H
#define fluxGroups_H

#include "fluxGroup.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// class forward declarations
class crossSectionSets;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                        Class fluxGroups Declaration
\*---------------------------------------------------------------------------*/
//- Scalar neutron flux fields for multiple broad group fluxes.
class fluxGroups
{
    // Private data

        //- Reference to the model mesh
        const fvMesh& mesh_;

        //- Number of broad energy groups
        label nGroups_;

        //- List of broad energy groups
        PtrList<fluxGroup> group_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fluxGroups(const fluxGroups&);

        //- Disallow default bitwise assignment
        void operator=(const fluxGroups&);

public:

    // Constructors

        //- Construct from dictionary entries
        fluxGroups
        (
            const fvMesh& mesh,
            const PtrList<entry>& groupEntries,
            crossSectionSets& sigma
        );


    // Destructor

        virtual ~fluxGroups();


    // Member Functions

        // Access

            //- Return a constant reference to the model mesh
            const fvMesh& mesh() const
            {
                return mesh_;
            }

            //- Return the number of broad energy groups
            label nGroups() const
            {
                return nGroups_;
            }

            //- Return a constant reference to a broad energy group by index
            const fluxGroup& group(const label i) const
            {
                return group_[i];
            }

            //- Return a reference to a broad energy group by index
            fluxGroup& group(const label i)
            {
                return group_[i];
            }

            //- Return the total scalar flux \f$ \sum_g' \sigma_g' \f$
            const tmp<volScalarField> phi() const;

            //- Return the total scalar flux time-derivative \f$ \frac{d}{dt} \sum_g' \sigma_g' \f$
            const tmp<volScalarField> ddtPhi() const;

            //- Return the neutron production \f$ P \f$ [/s]
            const tmp<volScalarField> nProduction() const;

            //- Return the fission rate \f$ F \f$ [/s]
            const tmp<volScalarField> fissionRate() const;

            //- Return the neutron flux multiplied by microscopic absorption cross-section for each isotope
            const tmp<volScalarField> isotopeSigmaPhi(const label i) const;

        // Update

            //- Update mesh leakages
            void updateLeakage();

            //- Update the fission product isotopic concentrations
            void updateIsotopes();

        // Edit

            //- Scale the neutron fluxes by a given factor
            void scaleFlux(const scalar& factor);

        // Write

            //- Write object
            virtual bool writeData(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
