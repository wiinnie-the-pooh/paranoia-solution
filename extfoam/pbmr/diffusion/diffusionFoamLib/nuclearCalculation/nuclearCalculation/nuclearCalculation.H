/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        nuclearCalculation.H
    Class:
        nuclearCalculation
    Description:
        Nuclear calculation control class
    SourceFiles:
        nuclearCalculation.C
-------------------------------------------------------------------------------------------------*/

#ifndef nuclearCalculation_H
#define nuclearCalculation_H

#include "fissionProducts.H"
#include "crossSectionSets.H"
#include "nuclearField.H"
#include "delayNeutrons.H"

#include "fvCFD.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations

/*---------------------------------------------------------------------------*\
                         Class crossSections Declaration
\*---------------------------------------------------------------------------*/
//- Nuclear calculation control class
class nuclearCalculation
:
    public IOdictionary
{
    // Private data

        //- Maximum number of inner flux iterations
        const label maxInnerIter_;

        //- Solution convergence criteria
        const scalar solutionTol_;

        //- Reference to mesh
        const fvMesh& mesh_;

        //- Fission product isotopes
        fissionProducts products_;

        //- Cross-sections
        autoPtr<crossSectionSets> sigma_;

        //- Basic nuclear fields
        nuclearField nuclField_;

        //- Delayed neutron fields
        delayNeutrons dnField_;

        //- Transient calculation switch
        Switch isTransient_;

        //- Requested global reactor power [W].
        //- Used for normalization suring steady-state calculations
        //- and fixed power transients.
        dimensionedScalar requestedPower_;

        //- Flag indicating that power is fixed for time-dependent calculations
        Switch powerIsFixed_;

        //- Output stream for scalar data
        OFstream timeData_;

        //- Flag for first call to solve
        bool isFirstCall_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        nuclearCalculation(const nuclearCalculation&);

        //- Disallow default bitwise assignment
        void operator=(const nuclearCalculation&);


public:

    // Static data members

        static const char* const dictionaryName;

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        nuclearCalculation
        (
            const fvMesh& mesh,
            const Switch& isTransient
        );

    // Destructor
        ~nuclearCalculation();


    // Member Functions

        // Access

            //- Return a reference to the nuclear fields
            const nuclearField& nuclField() const
            {
                return nuclField_;
            }

            //- Return a reference to the steady-state global reactor power [W].
            //- Used for normalization suring steady-state calculations
            //- and fixed power transients
            const dimensionedScalar& requestedPower() const
            {
                return requestedPower_;
            }

            //- Returns true if the power is fixed for time-dependent calculations
            /*! This is a flag introduced for load-follow type calculations where
                the global reactor power level is fixed and k-effective is calculated
                instead.
            */
            const Switch& powerIsFixed() const
            {
                return powerIsFixed_;
            }

        // Update

            //- Inner neutron flux iteration
            scalar solve();

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();

        // IO

            //- Write scalar values to file
            void writeScalars();

            //- Write object
            virtual bool writeData(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
