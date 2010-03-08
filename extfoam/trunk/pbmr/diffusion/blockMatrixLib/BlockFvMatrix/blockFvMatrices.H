/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        blockFvMatrices.H
    Class:
        blockFvMatrix
    Description:
        Typedefs for blockFvMatrix
    SourceFiles:
        blockFvMatrices.C
-------------------------------------------------------------------------------------------------*/

#ifndef blockFvMatrices_H
#define blockFvMatrices_H

#include "blockFvMatrix.H"
#include "blockLduMatrices.H"
#include "blockVector2Matrix.H"
#include "blockVector4Matrix.H"
#include "blockVector6Matrix.H"
#include "blockVector8Matrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef blockFvMatrix<vector2, tensor2> blockVector2FvMatrix;
typedef blockFvMatrix<vector, tensor> blockVectorFvMatrix;
typedef blockFvMatrix<vector4, tensor4> blockVector4FvMatrix;
typedef blockFvMatrix<vector6, tensor6> blockVector6FvMatrix;
typedef blockFvMatrix<vector8, tensor8> blockVector8FvMatrix;

typedef BlockSolverPerformance<vector2> blockVector2SolverPerformance;
typedef BlockSolverPerformance<vector> blockVectorSolverPerformance;
typedef BlockSolverPerformance<vector4> blockVector4SolverPerformance;
typedef BlockSolverPerformance<vector6> blockVector6SolverPerformance;
typedef BlockSolverPerformance<vector8> blockVector8SolverPerformance;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
