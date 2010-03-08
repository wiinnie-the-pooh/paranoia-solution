/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        diffusionCoefficients.C
-------------------------------------------------------------------------------------------------*/

#include "diffusionSolver.H"
#include "nuclearField.H"
#include "delayNeutrons.H"
#include "crossSectionSet.H"

#include "blockFvMatrices.H"
#include "bound.H"
#include "harmonicGaussLaplacianScheme.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::diffusionSolver::diffusionSolver
(
    nuclearField& nuclField,
    const delayNeutrons& dnField,
    const bool isTransient
)
:
    mesh_(nuclField.mesh()),
    nEqn_(nuclField.nGroups()),
    eqn_(nEqn_),
    phiBound_("minPhi", dimensionSet(0,-2,-1,0,0), 1e-6)
{
    if (nEqn_>0)
    {
        // Temporary prompt production factor
        volScalarField promptTerm
        (
            1/nuclField.globalParam().keff()
          * dnField.factor1()
        );

        // Temporary fixed source term
        volScalarField sourceTerm
        (
            nuclField.fixedSrc() - dnField.P()
        );

        if (isTransient)
        {
            sourceTerm = sourceTerm
              + dnField.factor0()*nuclField.P().oldTime();
        }
        else
        {
            sourceTerm = sourceTerm
              + dnField.factor0()*nuclField.P();
        }

        forAll(eqn_, g)
        {
            fluxGroup& thisGroup = nuclField.group(g);
            volScalarField& inGroupPhi = thisGroup.phi();
            const crossSectionSet& inGroupSigma = thisGroup.sigma();

            inGroupPhi.correctBoundaryConditions();

            eqn_.set
            (
                g,
                new PtrList<fvMatrix<scalar> >(nEqn_)
            );

            forAll(eqn_[g], g_)
            {
                if (g == g_) // Group equations
                {
                    // Construct matrix
                    eqn_[g].set
                    (
                        g_,
                        new fvMatrix<scalar>
                        (
                            fvm::SuSp
                            (
                                (
                                    inGroupSigma.Atot()
                                  + inGroupSigma.S()[g_]
                                ),
                                inGroupPhi
                            )
                            ==
                            sourceTerm * inGroupSigma.chi()
                        )
                    );

                    //- NOTE Forced harmonic interpolation
                    if (inGroupSigma.anisotropicD())
                    {
//                         eqn_[g][g_] -= fvm::laplacian
//                         (
//                             inGroupSigma.kD()/(3*inGroupSigma.Tr()),
//                             inGroupPhi,
//                             "laplacian(D,phi)"
//                         );
                        eqn_[g][g_] -= fv::harmonicGaussLaplacianScheme<scalar>(mesh_).fvmLaplacian
                        (
                            inGroupSigma.kD()/(3*inGroupSigma.Tr()),
                            inGroupPhi
                        );
                    }
                    else
                    {
//                         eqn_[g][g_] -= fvm::laplacian
//                         (
//                             1/(3*inGroupSigma.Tr()),
//                             inGroupPhi,
//                             "laplacian(D,phi)"
//                         );
                        eqn_[g][g_] -= fv::harmonicGaussLaplacianScheme<scalar>(mesh_).fvmLaplacian
                        (
                            1/(3*inGroupSigma.Tr()),
                            inGroupPhi
                        );
                    }

                    if (isTransient)    // Include time-dependent term
                    {                   // Production is implicit
                        eqn_[g][g_] +=
                        (
                            inGroupSigma.v() * fvm::ddt(inGroupPhi)
                          - fvm::SuSp
                            (
                                (
                                    promptTerm * inGroupSigma.chi()
                                      * inGroupSigma.nuF()
                                ),
                                inGroupPhi
                            )
                        );
                    }
                    else                // No time-dependent term
                    {                   // Production is explicit
                        eqn_[g][g_] -=
                            promptTerm * inGroupSigma.chi()
                              * inGroupSigma.nuF() * inGroupPhi;
                    }
                }
                else        // Group coupling terms
                {
                    const crossSectionSet& coupledSigma = nuclField.group(g_).sigma();

                    // Construct group coupling terms
                    eqn_[g].set
                    (
                        g_,
                        new fvMatrix<scalar>
                        (
                          - fvm::SuSp
                            (
                                (
                                    coupledSigma.S()[g]
                                ),
                                nuclField.group(g_).phi()
                            )
                        )
                    );

                    if (isTransient)    // Production is implicit
                    {
                        eqn_[g][g_] -=
                            fvm::SuSp
                            (
                                (
                                    promptTerm * inGroupSigma.chi()
                                      * coupledSigma.nuF()
                                ),
                                nuclField.group(g_).phi()
                            );
                    }
                    else                // Production is explicit
                    {
                        eqn_[g][g_] -=
                            promptTerm * inGroupSigma.chi()
                              * coupledSigma.nuF() * nuclField.group(g_).phi();
                    }
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::diffusionSolver::~diffusionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- BlockSolve
/*!
    Templated solve function for block-coupled equations which creates and solves
    the blockFvMatrix object and creates a solverPerformance object based on the
    blockSolverPerformance object returned.
    The maximimum component residual is returned.
    This function clears eqn_ and should only be called once.
    Called from Foam::diffusionFoam::diffusionSolver::solve.
*/
template<class vectorType, class tensorType>
Foam::lduMatrix::solverPerformance Foam::diffusionFoam::diffusionSolver::blockSolve()
{
    const word fieldName("phi");

    // Construct the blockFvMatrix object
    blockFvMatrix<vectorType,tensorType> blockEqn(fieldName, mesh_, eqn_);

    // Free memory before solve
    eqn_.clear();

    // Block-coupled solution
    BlockSolverPerformance<vectorType> blockSolverPerf = blockEqn.solve();

    scalar maxInitialResidual = 0;
    scalar maxFinalResidual = 0;

    for(label i=0; i< nEqn_; i++)
    {
        // Determine initial maximum component residual
        maxInitialResidual = max
        (
            maxInitialResidual,
            blockSolverPerf.initialResidual()[i]
        );

        // Determine final maximum component residual
        maxFinalResidual = max
        (
            maxFinalResidual,
            blockSolverPerf.finalResidual()[i]
        );
    }

    // Copy block solution performance into lduMatrix::solverPerformance
    return lduMatrix::solverPerformance
    (
        blockSolverPerf.solverName(),
        fieldName,
        maxInitialResidual,
        maxFinalResidual,
        blockSolverPerf.nIterations(),
        blockSolverPerf.converged(),
        blockSolverPerf.singular()
    );
}


//- Solve
/*!
    Solve the fvMatrix equations. This function essentially wraps the templated
    block solver, choosing the correct vectorN, tensorN templating depending on
    the number of coupled equations.
*/
Foam::lduMatrix::solverPerformance Foam::diffusionFoam::diffusionSolver::solve()
{
    lduMatrix::solverPerformance solverPerf;

    switch (nEqn_)
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            // Use the standard FOAM fvMatrix solver
            solverPerf = eqn_[0][0].solve();
            break;
        }
        case 2:
        {
            solverPerf = blockSolve<vector2,tensor2>();
            break;
        }
        case 3:
        {
            solverPerf = blockSolve<vector,tensor>();
            break;
        }
        case 4:
        {
            solverPerf = blockSolve<vector4,tensor4>();
            break;
        }
        case 6:
        {
            solverPerf = blockSolve<vector6,tensor6>();
            break;
        }
        case 8:
        {
            solverPerf = blockSolve<vector8,tensor8>();
            break;
        }
        default:
        {
            notImplemented
            (
                "Foam::lduMatrix::solverPerformance Foam::diffusionFoam::diffusionSolver::solve()"
                " for number of coupled equations = " + Foam::name(nEqn_)
            );
        }
    }

    return solverPerf;
}

//-----------------------------------------------------------------------------------------------//
