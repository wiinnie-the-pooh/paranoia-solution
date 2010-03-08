/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        diffusionSolver.H
    Class:
        diffusionSolver
    Description:
        Diffusion equation solver class.
    SourceFiles:
        diffusionSolver.C
-------------------------------------------------------------------------------------------------*/

#ifndef diffusionSolver_H
#define diffusionSolver_H

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class forward declarations
class nuclearField;
class delayNeutrons;

/*---------------------------------------------------------------------------*\
                         Class diffusionSolver Declaration
\*---------------------------------------------------------------------------*/
//- Diffusion equation solver class.
class diffusionSolver
{
    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- Number of coupled equations
        label nEqn_;

        //- Array of equation matrices
        PtrList<PtrList<fvMatrix<scalar> > > eqn_;

        //- Minimum allowed phi value.  Used in solution bounding
        const dimensionedScalar phiBound_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        diffusionSolver(const diffusionSolver&);

        //- Disallow default bitwise assignment
        void operator=(const diffusionSolver&);

        //- Solve
            template<class vectorType, class tensorType>
            lduMatrix::solverPerformance blockSolve();

public:

    // Constructors

        //- Construct from mesh and fission product isotope definitions
        diffusionSolver
        (
            nuclearField& nuclField,
            const delayNeutrons& dnField,
            const bool isTransient
        );

    // Destructor
        ~diffusionSolver();


    // Member Functions

        // Access

            //- Return reference to equation matrices
            PtrList<PtrList<fvMatrix<scalar> > >& eqn()
            {
                return eqn_;
            }

        // Update

            //- Solve
            lduMatrix::solverPerformance solve();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
