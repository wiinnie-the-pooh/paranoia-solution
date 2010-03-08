/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        delayNeutrons.H
    Class:
        delayNeutrons
    Description:
        Class containing a list of delay neutron precursor groups and related properties
    SourceFiles:
        delayNeutrons.C
-------------------------------------------------------------------------------------------------*/

#ifndef delayNeutrons_H
#define delayNeutrons_H

#include "IOdictionary.H"
#include "delayNeutronGroup.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

/*---------------------------------------------------------------------------*\
                         Class delayNeutrons Declaration
\*---------------------------------------------------------------------------*/
//- Class containing a list of delay neutron precursor groups and related properties
class delayNeutrons
:
    public IOdictionary
{
    // Private data

        //- List of delay neutron groups
        PtrList<delayNeutronGroup> group_;

        //- New prompt neutron production factor
        volScalarField factor1_;

        //- Old prompt neutron production factor
        volScalarField factor0_;

        //- Delayed neutron production
        volScalarField P_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        delayNeutrons(const delayNeutrons&);

        //- Disallow default bitwise assignment
        void operator=(const delayNeutrons&);


public:

    // Constructors

        //- Construct from components
        delayNeutrons(const fvMesh& mesh);

    // Destructor

        ~delayNeutrons();


    // Member Functions

        // Access

            //- Return list of delay neutron groups
            const PtrList<delayNeutronGroup>& group() const
            {
                return group_;
            }

            //- Return a reference to the new prompt neutron production factor \f$ \zeta_1 \f$
            const volScalarField& factor1() const
            {
                return factor1_;
            }

            //- Return a reference to the old prompt neutron production factor \f$ \zeta_0 \f$
            const volScalarField& factor0() const
            {
                return factor0_;
            }

            //- Return a reference to the delayed neutron production \f$ Q_d \f$
            const volScalarField& P() const
            {
                return P_;
            }


        // Update state

            //- Update delayed neutron production terms
            void updateProductionTerms(const bool isTransient);

            //- Updated delayed neutron precursor concentrations
            void updateConcentrations
            (
                const volScalarField& P,
                bool isTransient
            );

            //- Store the old time values
            void storeOldTime();

            //- Store the previous iteration values
            void storePrevIter();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
