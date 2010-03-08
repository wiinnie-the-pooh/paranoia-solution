/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        blockFvMatrix.H
    Class:
        blockFvMatrix
    Description:
        Wrapper class to the blockLduMatrix class supplied by H. Jasak in Dec 2007.
    SourceFiles:
        blockFvMatrix.C
-------------------------------------------------------------------------------------------------*/

#ifndef blockFvMatrix_H
#define blockFvMatrix_H

#include "fvCFD.H"
#include "blockLduMatrices.H"
#include "blockLduSolvers.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                         Class blockFvMatrix Declaration
\*---------------------------------------------------------------------------*/
//- Wrapper class to the blockLduMatrix class supplied by H. Jasak in Dec 2007.
//- This is not completely generic, a block point-implicit coupling is assumed.
//- The number of coupled equations is determined by the vectorType and
//- tensorType classes.
template<class vectorType, class tensorType>
class blockFvMatrix
{
    // Private data

        //- psi name
        word name_;

        //- Number of coupled equations
        label nCoupled_;

        //- Size of the main diagonal
        label diagSize_;

        //- Size of the upper/lower matrices
        label ulSize_;

        //- Reference to the mesh
        const fvMesh& mesh_;

        //- Block matrix
        BlockLduMatrix<vectorType> blockMatrix_;

        //- Block source
        Field<vectorType> blockSource_;

        //- Block psi variable
        Field<vectorType> blockPsi_;

        //- List of references to psi variables
//        PtrList<reference<volScalarField> > psi_;
        List<volScalarField*> psi_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        blockFvMatrix(const blockFvMatrix&);

        //- Disallow default bitwise assignment
        void operator=(const blockFvMatrix&);


public:

    //- Runtime type information
    TypeName("blockFvMatrix");

    // Constructors

        //- Construct the blockFvMatrix from a mesh and array of fvMesh objects
        blockFvMatrix
        (
            const word name,
            const fvMesh& mesh,
            PtrList<PtrList<fvMatrix<scalar> > >& srcFvMatrix
        );

    // Destructor
        virtual ~blockFvMatrix();


    // Member Functions

        // Access

        //- Return constant reference to the block scalar field
        inline const Field<vectorType>& blockPsi()
        {
            return blockPsi_;
        }

        // Update

        //- Perform the block solution
        BlockSolverPerformance<vectorType> solve();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "blockFvMatrix.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
