/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        temperatureCalculation.H
    Class:
        temperatureCalculation
    Description:
        Temperature calculation dummy control class. Provides moderator and fuel temperatures
        for cross-section calculations
    SourceFiles:
        temperatureCalculation.C
-------------------------------------------------------------------------------------------------*/

#ifndef temperatureCalculation_H
#define temperatureCalculation_H

#include "fvCFD.H"
#include "autoPtr.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations

/*---------------------------------------------------------------------------*\
                         Class temperatureCalculation Declaration
\*---------------------------------------------------------------------------*/
//- Temperature calculation dummy control class. Provides moderator and
//- fuel temperatures for cross-section calculations
class temperatureCalculation
:
    public IOdictionary
{
protected:
    // Type declarations
        typedef const volScalarField constVolScalarField;

    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- Moderator temperature
        volScalarField Tmod_;

        //- Fuel temperature
        volScalarField Tfuel_;

//=====================================
// Moderator thermal properties
// Simplified model variables
        volScalarField kMod_;   // Thermal conductivity W/m^2/K
        volScalarField rhoMod_; // Density kg/m^3
        volScalarField cpMod_;  // Specific heat capacity W/m^3/K

        const scalar puebh_;    // Fuel kernel overheating parameter
//=====================================

        //- Reference to nuclear heat source field
        mutable constVolScalarField* heatSrc_;

        //- Transient calculation switch
        Switch isTransient_;

        //- Flag for first call to solve
        bool isFirstCall_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        temperatureCalculation(const temperatureCalculation&);

        //- Disallow default bitwise assignment
        void operator=(const temperatureCalculation&);


public:

    // Static data members

        static const char* const dictionaryName;

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        temperatureCalculation
        (
            const fvMesh& mesh,
            const Switch& isTransient
        );

    // Destructor
        ~temperatureCalculation();


    // Member Functions

        // Access
            //- Return reference to heatSrc_
            const volScalarField& heatSrc() const;

            //- Return reference to Tfuel_
            const volScalarField& Tfuel() const;

            //- Return reference to Tmod_
            const volScalarField& Tmod() const;

        // Update
            //- Temperature solution
            scalar solve();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
