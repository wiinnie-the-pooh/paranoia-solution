/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        blockFvMatrix.C
-------------------------------------------------------------------------------------------------*/

#include "blockFvMatrix.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct the blockFvMatrix from a mesh and array of fvMesh objects
/*! A block point-implicit coupling is assumed.
    The number of coupled equations is determined by the vectorType and
    tensorType classes.
*/
template<class vectorType, class tensorType>
Foam::blockFvMatrix<vectorType,tensorType>::blockFvMatrix
(
    const word name,
    const fvMesh& mesh,
    PtrList<PtrList<fvMatrix<scalar> > >& srcFvMatrix
)
:
    nCoupled_(srcFvMatrix.size()),
    diagSize_(mesh.lduAddr().size()),
    ulSize_(mesh.lduAddr().lowerAddr().size()),
    mesh_(mesh),
    blockMatrix_(mesh),
    blockSource_(diagSize_, pTraits<vectorType>::zero),
    blockPsi_(diagSize_, pTraits<vectorType>::zero),
    psi_(nCoupled_)
{
    name_ = name;

    //Todo - Check that tensorType and vectorType have matching sizes
    //       Possibly replace vectorType & tensorType with Cmpt & length
    if (nCoupled_ > vectorType::nComponents)
    {
        FatalErrorIn("Foam::blockFvMatrix<tensorType, vectorType>::blockFvMatrix") <<
            "More than " << vectorType::nComponents << " coupled equations not supported for tensorType " <<
            tensorType::typeName << " and vectorType " << vectorType::typeName << Foam::abort(FatalError);
    }

    // Working temporary fields
    Field<vectorType> blockUpper(ulSize_, pTraits<vectorType>::zero);
    // For nGroups < vectorType::nComponents fill the dummy diagonal with 1... will yield
    // zero solution for psi. Requires variable size block coefficient before
    // this workaround can be bypassed
    tensorType identity(pTraits<tensorType>::zero);
    for (label i = 0; i < tensorType::nComponents; i += vectorType::nComponents+1)
    {
        identity[i] = scalar(1);
    }
    Field<tensorType> blockDiag(diagSize_, identity);

    // Pack Coefficients into block fields
    label cmptID = 0;

    for (label i=0; i<nCoupled_; i++)
    {
        fvMatrix<scalar>& curFvMatrix = srcFvMatrix[i][i];
        volScalarField& curPsi = curFvMatrix.psi();

        // Pack initial psi guess
//        psi_.set(i,new reference<volScalarField>(curPsi));
        psi_[i] = &curPsi;

        blockPsi_.replace(i, curPsi);

        // Pack coefficients into upper
        blockUpper.replace(i, curFvMatrix.upper());


        // Pack coefficients into diagonal
        for (label j=0; j<nCoupled_; j++)
        {
            Field<scalar> curD(srcFvMatrix[i][j].D());

            blockDiag.replace(cmptID, curD);

            cmptID ++;
        }
        //===========================
        // I.C. 2008/03/27
        // Move to next row in tensorType if nCoupled_ is less than vectorType::nComponents
        cmptID += vectorType::nComponents-nCoupled_;

        // Pack coefficients into source
        scalarField curSource(diagSize_);
        for (label j=0; j<nCoupled_; j++)
        {
            curSource += srcFvMatrix[i][j].source();
            srcFvMatrix[i][j].addBoundarySource(curSource);
        }
        blockSource_.replace(i, curSource);
    }

    // Construct block matrix object
    blockMatrix_.diag() = blockDiag;
    blockMatrix_.upper() = blockUpper;

//    Info << blockDiag << endl;
//    Info << blockUpper << endl;
//    Info << blockSource_ << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class vectorType, class tensorType>
Foam::blockFvMatrix<vectorType,tensorType>::~blockFvMatrix()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Perform the block solution
template<class vectorType, class tensorType>
Foam::BlockSolverPerformance<vectorType>
Foam::blockFvMatrix<vectorType,tensorType>::solve()
{
    BlockSolverPerformance<vectorType> solverPerf =
        BlockLduSolver<vectorType>::New
        (
            name_,
            blockMatrix_,
            mesh_.solver(name_)
        )->solve(blockPsi_, blockSource_);

    forAll(psi_, i)
    {
        scalarField& curField = psi_[i]->internalField();
        curField = blockPsi_.component(i);

        psi_[i]->correctBoundaryConditions();
    }

    // Todo - Move this above the solution step
    Info << solverPerf.solverName();
    Info << ": Solving for " << name_ << flush;
    Info << ", Initial residual = " << mag(solverPerf.initialResidual());
    Info << ", Final residual = " << mag(solverPerf.finalResidual());

    if (solverPerf.converged())
    {
        Info << ", No iterations " << solverPerf.nIterations() << endl;
    }
    else
    {
        Info << ", Not converged" << endl;
    }

//    Info << blockPsi_ << endl;

    return solverPerf;
}


//-----------------------------------------------------------------------------------------------//
