/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fissionProducts.H
    Class:
        fissionProducts
    Description:
        Container class for fission products
    SourceFiles:
        fissionProducts.C
-------------------------------------------------------------------------------------------------*/

#ifndef fissionProducts_H
#define fissionProducts_H

#include "IOdictionary.H"
#include "isotope.H"
#include "decayChain.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class nuclearField;

/*---------------------------------------------------------------------------*\
                      Class fissionProducts Declaration
\*---------------------------------------------------------------------------*/
//- Container class for fission products
class fissionProducts
:
    public IOdictionary
    
{
    // Private data

        //- List of isotopes
        PtrList<isotope> isotope_;

        //- List of decay chains
        PtrList<decayChain> decayChain_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fissionProducts(const fissionProducts&);

        //- Disallow default bitwise assignment
        void operator=(const fissionProducts&);


public:

    // Constructors

        //- Construct from nuclear field
        fissionProducts(const fvMesh& mesh);


    // Destructor

        ~fissionProducts();


    // Member Functions

        // Access

            //- Return a list of isotopes
            const PtrList<isotope>& isotopes() const
            {
                return isotope_;
            }

            //- Return the constant reference to an isotope by name
            isotope& isotopeByName(const word name);

            //- Return a list of decay chains
            const PtrList<decayChain>& decayChains() const
            {
                return decayChain_;
            }

            //- Return the number of defined isotopes
            label nIsotopes() const
            {
                return isotope_.size();
            }

            //- Return the number of defined decay chains
            label nDecayChains() const
            {
                return decayChain_.size();
            }

        // Update

            //- Update the fission product concentrations for all decay chains
            void updateConcentrations
            (
                const nuclearField& nuclField,
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
