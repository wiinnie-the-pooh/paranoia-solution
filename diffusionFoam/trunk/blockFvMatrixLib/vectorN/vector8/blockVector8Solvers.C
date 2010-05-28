/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-6 H. Jasak All rights reserved
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

\*---------------------------------------------------------------------------*/

#include "vector8Field.H"
#include "tensor8Field.H"
#include "ExpandTensorN.H"
#include "ExpandTensorNField.H"
#include "blockLduMatrices.H"
#include "addToRunTimeSelectionTable.H"

#include "blockLduPrecons.H"
#include "BlockNoPrecon.H"
#include "blockDiagonalPrecons.H"
#include "blockGaussSeidelPrecons.H"
#include "BlockCholeskyPrecon.H"

#include "blockLduSmoothers.H"
#include "blockGaussSeidelSmoothers.H"
#include "BlockILUSmoother.H"

#include "blockLduSolvers.H"
#include "BlockDiagonalSolver.H"
#include "BlockBiCGStabSolver.H"
#include "BlockCGSolver.H"
#include "BlockGaussSeidelSolver.H"
#include "BlockGMRESSolver.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Preconditioners
typedef BlockLduPrecon<vector8> blockVector8Precon;
defineNamedTemplateTypeNameAndDebug(blockVector8Precon, 0);
defineTemplateRunTimeSelectionTable(blockVector8Precon, dictionary);

typedef BlockNoPrecon<vector8> blockNoPreconVector8;
makeBlockPrecon(blockVector8Precon, blockNoPreconVector8);

typedef BlockDiagonalPrecon<vector8> blockDiagonalPreconVector8;
makeBlockPrecon(blockVector8Precon, blockDiagonalPreconVector8);

typedef BlockGaussSeidelPrecon<vector8> blockGaussSeidelPreconVector8;
makeBlockPrecon(blockVector8Precon, blockGaussSeidelPreconVector8);

typedef BlockCholeskyPrecon<vector8> blockCholeskyPreconVector8;
makeBlockPrecon(blockVector8Precon, blockCholeskyPreconVector8);


// Smoothers
typedef BlockLduSmoother<vector8> blockVector8Smoother;
defineNamedTemplateTypeNameAndDebug(blockVector8Smoother, 0);
defineTemplateRunTimeSelectionTable(blockVector8Smoother, dictionary);

typedef BlockGaussSeidelSmoother<vector8> blockGaussSeidelSmootherVector8;
makeBlockSmoother(blockVector8Smoother, blockGaussSeidelSmootherVector8);

typedef BlockILUSmoother<vector8> blockILUSmootherVector8;
makeBlockSmoother(blockVector8Smoother, blockILUSmootherVector8);


// Solvers
typedef BlockLduSolver<vector8> blockVector8Solver;
defineNamedTemplateTypeNameAndDebug(blockVector8Solver, 0);
defineTemplateRunTimeSelectionTable
(
    blockVector8Solver,
    symMatrix
);

defineTemplateRunTimeSelectionTable
(
    blockVector8Solver,
    asymMatrix
);

typedef BlockDiagonalSolver<vector8> blockDiagonalSolverVector8;
defineNamedTemplateTypeNameAndDebug(blockDiagonalSolverVector8, 0);

typedef BlockBiCGStabSolver<vector8> blockBiCGStabSolverVector8;
makeBlockSolverTypeName(blockBiCGStabSolverVector8);
addSolverToBlockMatrix(Vector8, blockBiCGStabSolverVector8, symMatrix);
addSolverToBlockMatrix(Vector8, blockBiCGStabSolverVector8, asymMatrix);

typedef BlockCGSolver<vector8> blockCGSolverVector8;
makeBlockSolverTypeName(blockCGSolverVector8);
addSolverToBlockMatrix(Vector8, blockCGSolverVector8, symMatrix);

typedef BlockGaussSeidelSolver<vector8> blockGaussSeidelSolverVector8;
makeBlockSolverTypeName(blockGaussSeidelSolverVector8);
addSolverToBlockMatrix(Vector8, blockGaussSeidelSolverVector8, symMatrix);

typedef BlockGMRESSolver<vector8> blockGMRESSolverVector8;
makeBlockSolverTypeName(blockGMRESSolverVector8);
addSolverToBlockMatrix(Vector8, blockGMRESSolverVector8, symMatrix);
addSolverToBlockMatrix(Vector8, blockGMRESSolverVector8, asymMatrix);



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
