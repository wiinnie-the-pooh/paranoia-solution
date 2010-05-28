/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        nuclearField.H
    Class:
        nuclearField
    Description:
        Nuclear calculation definition class, combining flux & power profiles for multiple
        energy groups.
    SourceFiles:
        nuclearField.C
-------------------------------------------------------------------------------------------------*/

#ifndef nuclearField_H
#define nuclearField_H

#include "fluxGroups.H"
#include "globalNuclearParameters.H"
#include "heatProduction.H"
#include "crossSectionSets.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Class forward declarations
class fissionProducts;
class delayNeutrons;

/*---------------------------------------------------------------------------*\
                        Class nuclearField Declaration
\*---------------------------------------------------------------------------*/
//- Nuclear calculation definition class, combining flux & power profiles
//- for multiple energy groups.
class nuclearField
:
    public IOdictionary,
    public fluxGroups
{
    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- Global nuclear parameters
        globalNuclearParameters globalParam_;

        //- Nuclear heat production
        heatProduction heatProduction_;

        //- Fission Rate \f$ F \f$
        volScalarField F_;

        //- Neutron production \f$ \frac{1}{k} P \f$
        volScalarField P_;

        //- Fixed homogenous neutron source \f$ S_\phi \f$
        dimensionedScalar fixedSrc_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        nuclearField(const nuclearField&);

        //- Disallow default bitwise assignment
        void operator=(const nuclearField&);


public:

    // Constructors

        //- Construct from dictionary
        nuclearField(const fvMesh& mesh, crossSectionSets& sigma);


    // Destructor

        ~nuclearField();


    // Member Functions

        // Access

            //- Return a constant reference to the model mesh
            const fvMesh& mesh() const
            {
                return mesh_;
            }

            //- Return constant reference to global nuclear parameters
            globalNuclearParameters& globalParam()
            {
                return globalParam_;
            }

            //- Return constant reference to heat production
            heatProduction& power()
            {
                return heatProduction_;
            }

            const heatProduction& power() const
            {
                return heatProduction_;
            }

            //- Return the fission rate \f$ F /\f$
            const volScalarField& F() const
            {
                return F_;
            }

            //- Return the neutron production \f$ \frac{1}{k} P \f$
            const volScalarField& P() const
            {
                return P_;
            }

            //- Return a reference to the fixed homogenous neutron source \f$ S_\phi \f$
            const dimensionedScalar& fixedSrc() const
            {
                return fixedSrc_;
            }

        // Update

            //- Update fission rates
            void updateFissionRate();

            //- Update the neutron production field
            void updateNeutronProduction();

            //- Update k-effective
            void updateKeff
            (
                const delayNeutrons& dnField,
                const bool isTransient
            );

            //- Update reactor power
            void updatePower();

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();

        // Edit

            //- Normalise the total reactor power to the steady-state value
            void normalizePower(const dimensionedScalar& newPower);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
