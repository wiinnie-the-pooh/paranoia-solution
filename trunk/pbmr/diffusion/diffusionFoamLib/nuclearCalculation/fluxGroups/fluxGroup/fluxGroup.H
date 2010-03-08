/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fluxGroup.H
    Class:
        fluxGroup
    Description:
        Class defining the neutronic state and properties of a single broad energy group
    SourceFiles:
        fluxGroup.C
-------------------------------------------------------------------------------------------------*/

#ifndef fluxGroup_H
#define fluxGroup_H

#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class crossSectionSet;

/*---------------------------------------------------------------------------*\
                           Class fluxGroup Declaration
\*---------------------------------------------------------------------------*/
//- Class defining the neutronic state and properties of a single broad energy group
class fluxGroup
{
    // Private data

        //- Broad group index
        label index_;

        //- Reference to the associated broad-group cross-sections
        crossSectionSet& sigma_;

        //- Neutron flux field \f$ \phi_g \f$
        volScalarField phi_;

        //- Mesh leakage field \f$ DB^2\phi_g \f$
        volScalarField leakage_;

        //- Mesh buckling field \f$ DB^2 \f$
        volScalarField buckling_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fluxGroup(const fluxGroup&);

        //- Disallow default bitwise assignment
        void operator=(const fluxGroup&);


public:

    // Constructors

        //- Construct from components
        fluxGroup
        (
            const label index,
            const fvMesh& mesh,
            crossSectionSet& sigma,
            const dictionary& dict
        );


    // Destructor

        virtual ~fluxGroup();


    // Member Functions

        // Access

            //- Return a constant reference to the associated broad group cross-section set
            const crossSectionSet& sigma() const
            {
                return sigma_;
            }

            //- Return a reference to the associated broad group cross-section set
            crossSectionSet& sigma()
            {
                return sigma_;
            }

            //- Return a reference to the neutron flux field \f$ \phi_g \f$
            const volScalarField& phi() const
            {
                return phi_;
            }

            //- Return a non-constant reference to the neutron flux field \f$ \phi_g \f$
            volScalarField& phi()
            {
                return phi_;
            }

            //- Return a reference to the neutron leakage field \f$ DB^2\phi_g \f$
            const volScalarField& leakage() const
            {
                return leakage_;
            }

            //- Return a reference to the neutron leakage field \f$ DB^2\phi_g \f$
            volScalarField& leakage()
            {
                return leakage_;
            }

            //- Return a reference to the buckling field \f$ DB^2 \f$
            const volScalarField& buckling() const
            {
                return buckling_;
            }

            //- Return the fission rate
            const tmp<volScalarField> fissionRate() const;

            //- Return the neutron production
            const tmp<volScalarField> nProduction() const;

            //- Return the neutron flux multiplied by the microscopic absorption cross-section for a given isotope
            const tmp<volScalarField> isotopeSigmaPhi(const label i) const;

        // Update

            //- Scale the neutron flux and leakage by a given factor
            void scale(const scalar& scaleFactor);

            //- Calculate updated buckling values
            void updateLeakage();

        // Write

            //- Force the writing of fields to file
            void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
