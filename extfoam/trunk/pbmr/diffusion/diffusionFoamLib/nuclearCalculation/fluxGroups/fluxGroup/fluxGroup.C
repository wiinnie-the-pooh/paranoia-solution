/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fluxGroup.C
-------------------------------------------------------------------------------------------------*/

#include "fluxGroup.H"
#include "crossSectionSet.H"
#include "fvCFD.H"
#include "fvMatrices.H"
//#include "fvc.H"
#include "harmonicGaussLaplacianScheme.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::fluxGroup::fluxGroup
(
    const label index,
    const fvMesh& mesh,
    crossSectionSet& sigma,
    const dictionary& dict
)
:
    index_(index),
    sigma_(sigma),
    phi_
    (
        IOobject
        (
            word(dict.lookup("neutronFlux")),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    leakage_
    (
        IOobject
        (
            word(dict.lookup("leakage")),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimensionSet(0, -3, -1, 0, 0, 0, 0), 0)
    ),
    buckling_
    (
        IOobject
        (
            word(dict.lookup("buckling")),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimensionSet(0, -1, 0, 0, 0, 0, 0), 0)
    )
{
    Info << "Constructing broad flux group from dictionary " << dict.name() << endl;
    updateLeakage();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::fluxGroup::~fluxGroup()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Scale the neutron flux and leakage by a given factor
void Foam::diffusionFoam::fluxGroup::scale(const scalar& scaleFactor)
{
    phi_ *= scaleFactor;
    leakage_ *= scaleFactor;
}


//- Calculate updated neutron leakage values
void Foam::diffusionFoam::fluxGroup::updateLeakage()
{
    const fvMesh& mesh = phi_.mesh();

    //- NOTE Forced harmonic interpolation
    if (sigma_.anisotropicD())
    {
        leakage_ = fv::harmonicGaussLaplacianScheme<scalar>(mesh).fvcLaplacian
        (
            sigma_.kD()/(3*sigma_.Tr()),
            phi_
        );
    }
    else
    {
        leakage_ = fv::harmonicGaussLaplacianScheme<scalar>(mesh).fvcLaplacian
        (
            1/(3*sigma_.Tr()),
            phi_
        );
    }

    //    leakage_ = fvc::laplacian(1/(3*sigma_.Tr()), phi_, "laplacian(D,phi)");

    buckling_.internalField() = leakage_.internalField()/phi_.internalField();
}


//- Return the fission rate
const Foam::tmp<volScalarField> Foam::diffusionFoam::fluxGroup::fissionRate() const
{
    return sigma_.F()*phi_;
}


//- Return the neutron production
const Foam::tmp<volScalarField> Foam::diffusionFoam::fluxGroup::nProduction() const
{
    return sigma_.nuF()*phi_;
}


//- Return the neutron flux multiplied by the microscopic absorption cross-section for a given isotope
const Foam::tmp<volScalarField> Foam::diffusionFoam::fluxGroup::isotopeSigmaPhi(const label i) const
{
    return sigma_.isotopeSigma(i)*phi_;
}


//- Force the writing of fields to file
void Foam::diffusionFoam::fluxGroup::write()
{
    phi_.write();
}

// ************************************************************************* //
