/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        heatProduction.H
    Class:
        heatProduction
    Description:
        Nuclear heat production class
    SourceFiles:
        heatProduction.C
-------------------------------------------------------------------------------------------------*/

#ifndef heatProduction_H
#define heatProduction_H

#include "IOdictionary.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

/*---------------------------------------------------------------------------*\
                         Class crossSections Declaration
\*---------------------------------------------------------------------------*/
//- Nuclear heat production class
class heatProduction
{
    // Private data

        //- Associated dictionary
        IOdictionary dict_;

        //- Energy per fission \f$ E_f \f$ [J/fission]
        volScalarField Ef_;

        //- Power density field \f$ Q''' \f$ [W/m^3]
        volScalarField powerDensity_;

        //- Global reactor power \f$ P_{tot} \f$ [W]
        dimensionedScalar globalPower_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        heatProduction(const heatProduction&);

        //- Disallow default bitwise assignment
        void operator=(const heatProduction&);


public:

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        heatProduction
        (
            const fvMesh& mesh,
            const word& dictName
        );

    // Destructor
        virtual ~heatProduction();


    // Member Functions

        // Access

            //- Return the power density \f$ Q''' \f$
            const volScalarField& powerDensity() const
            {
                return powerDensity_;
            }

            //- Return a reference to the global reactor power \f$ P_{tot} \f$ [W]
            const dimensionedScalar& globalPower() const
            {
                return globalPower_;
            }

        // Update state

            //- Update the power density fields and global reactor power
            void updatePower(const volScalarField& F);

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();

        // Modify

            //- Scale the nuclear power
            void scale(const scalar& factor);

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

//-----------------------------------------------------------------------------------------------//
