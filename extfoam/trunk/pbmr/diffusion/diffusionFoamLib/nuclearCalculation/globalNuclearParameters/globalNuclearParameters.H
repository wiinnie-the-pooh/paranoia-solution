/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        globalNuclearParameters.H
    Class:
        globalNuclearParameters
    Description:
        Global reactor parameters associated with the nuclear calculation.
    SourceFiles:
        globalNuclearParameters.C
-------------------------------------------------------------------------------------------------*/

#ifndef globalNuclearParameters_H
#define globalNuclearParameters_H

#include "fvCFD.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Class forward declarations
class nuclearField;
class delayNeutrons;
class fissionProducts;

/*---------------------------------------------------------------------------*\
                        Class globalNuclearParameters Declaration
\*---------------------------------------------------------------------------*/
//- Global reactor parameters associated with the nuclear calculation.
class globalNuclearParameters
:
    public IOdictionary
{
    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- K-effective bounding value
        const scalar keffBound_;

        //- K-effective value \f$ k \f$
        scalar keff_;

        //- Old k-effective value
        scalar keff0_;

        //- Previous iteration k-effective value
        scalar keffPrevIter_;

        //- Inverse reactor period \f$ \omega \f$ [/s]
        dimensionedScalar omega_;

        //- Global neutron production [/s]
        dimensionedScalar globalProd_;

        //- Global neutron loss [/s]
        dimensionedScalar globalLoss_;

        //- Global neutron delayed production [/s]
        dimensionedScalar globalDelayed_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        globalNuclearParameters(const globalNuclearParameters&);

        //- Disallow default bitwise assignment
        void operator=(const globalNuclearParameters&);


public:

    // Constructors

        //- Construct from dictionary
        globalNuclearParameters(const fvMesh& mesh, const word& dictName);


    // Destructor

        ~globalNuclearParameters();


    // Member Functions

        // Access

            //- Return a reference to the k-effective value \f$ k \f$
            const scalar& keff() const
            {
                return keff_;
            }

            //- Return a reference to the previous timestep k-effective value
            const scalar& keff0() const
            {
                return keff0_;
            }

            //- Return a reference to the previous iteration k-effective value
            const scalar& keffPrevIter() const
            {
                return keffPrevIter_;
            }

            //- Return a reference to the inverse reactor period \f$ \omega \f$ [/s]
            const dimensionedScalar& omega() const
            {
                return omega_;
            }

        // Update

            //- Update the global reactor multiplication constant k-effective
            void updateKEffective
            (
                const nuclearField& nuclField,
                const delayNeutrons& dnField,
                const bool isTransient
            );

            //- Update the inverse reactor period
            void updateReactorPeriod
            (
                const nuclearField& nuclField
            );

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();

        // Edit

            //- Scale by a given factor
            void scale(const scalar& factor);

            //- Insert an external reactivity
            void insertReactivity(scalar rho);


        // IO

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
