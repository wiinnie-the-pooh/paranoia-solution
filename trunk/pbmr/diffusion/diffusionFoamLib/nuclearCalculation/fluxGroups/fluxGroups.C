/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fluxGroups.C
-------------------------------------------------------------------------------------------------*/

#include "fluxGroups.H"
#include "crossSectionSets.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diffusionFoam::fluxGroups::fluxGroups
(
    const fvMesh& mesh,
    const PtrList<entry>& groupEntries,
    crossSectionSets& sigma
)
:
    mesh_(mesh),
    group_()
{
    Info << "Setting up broad flux groups" << endl;

    // Read fluxGroups entries
    nGroups_ = groupEntries.size();

    if (nGroups_ != sigma.nGroups())
    {
        FatalErrorIn("Foam::diffusionFoam::fluxGroups::fluxGroups") <<
            "Group structure defined in IOdictionary does not match supplied cross-sections" <<
            Foam::abort(FatalError);
    }

    group_.setSize(nGroups_);

    forAll(groupEntries, g)
    {
        group_.set
        (
            g,
            new fluxGroup(g, mesh, sigma.group(g), groupEntries[g].dict())
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diffusionFoam::fluxGroups::~fluxGroups()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


//- Return the total scalar flux \f$ \sum_g' \sigma_g' \f$
const Foam::tmp<Foam::volScalarField> Foam::diffusionFoam::fluxGroups::phi() const
{
    tmp<volScalarField> tPhi
    (
        new volScalarField
        (
            IOobject
            (
                "0",
                mesh_.time().timeName(),
                mesh_
            ),
            mesh_,
            dimensionedScalar("0", dimensionSet(0, -2, -1, 0, 0), 0.0)
        )
    );

    forAll(group_, g)
    {
        tPhi = tPhi + group_[g].phi();
    }

    return tPhi;
}


//- Return the total scalar flux time-derivative \f$ \frac{d}{dt} \sum_g' \sigma_g' \f$
const Foam::tmp<Foam::volScalarField> Foam::diffusionFoam::fluxGroups::ddtPhi() const
{
    tmp<volScalarField> tDdtPhi
    (
        new volScalarField
        (
            IOobject
            (
                "0",
                mesh_.time().timeName(),
                mesh_
            ),
            mesh_,
            dimensionedScalar("0",  dimensionSet(0, -2, -2, 0, 0), 0.0)
        )
    );

    forAll(group_, g)
    {
        tDdtPhi = tDdtPhi + fvc::ddt(group_[g].phi());
    }

    return tDdtPhi;
}


//- Return the neutron production \f$ P \f$ [/s]
/*!
    The total neutron production is calculated as:
    \f[
        \frac{1}{k} P^{'''} = \sum_g \frac{1}{k} \nu\Sigma_{f,g} \phi_g
    \f]
    where g is the broad energy group index.
    Note: The production here includes the k-effective factor.
*/
const Foam::tmp<Foam::volScalarField> Foam::diffusionFoam::fluxGroups::nProduction() const
{
    tmp<volScalarField> tP
    (
        new volScalarField
        (
            IOobject
            (
                "P",
                mesh_.time().timeName(),
                mesh_
            ),
            mesh_,
            dimensionedScalar("0", dimless/dimVol/dimTime, 0.0)
        )
    );

    forAll(group_, g)
    {
        tP = tP + group_[g].nProduction();
    }

    return tP;
}


//- Return the fission rate \f$ F \f$ [/s]
/*!
    The fission rate is calculated as:
    \f[
        F^{'''} = \sum_g \Sigma_{f,g} \phi_g
    \f]
    where g is the broad energy group index.
*/
const Foam::tmp<Foam::volScalarField> Foam::diffusionFoam::fluxGroups::fissionRate() const
{
    tmp<volScalarField> tF
    (
        new volScalarField
        (
            IOobject
            (
                "F",
                mesh_.time().timeName(),
                mesh_
            ),
            mesh_,
            dimensionedScalar("0", dimless/dimVol/dimTime, 0.0)
        )
    );

    forAll(group_, g)
    {
        tF = tF + group_[g].fissionRate();
    }

    return tF;
}


//- Return the neutron flux multiplied by microscopic absorption cross-section for each isotope
/*!
    \f[
        \left(\sigma\phi\right)_i = \sum_{g}{\sigma_{i,g} \phi_g}
    \f]
    where i is the isotope index and g is the broad energy group index.
    The method isotopeSigmaPhi is called for each flux group to calculate the sum.
*/
const Foam::tmp<Foam::volScalarField> Foam::diffusionFoam::fluxGroups::isotopeSigmaPhi(const label i) const
{
    label nGroups = group_.size();

    if (nGroups==0)
    {
        FatalErrorIn("Foam::diffusionFoam::nuclearField::isotopeSigmaPhi") <<
                "No flux fields found" <<
                Foam::abort(FatalError);
    }

    // First energy group
    tmp<volScalarField> tmpSigmaPhi(group_[0].isotopeSigmaPhi(i));

    volScalarField& sigmaPhi = tmpSigmaPhi();

    // Remaining energy groups
    if (nGroups>1)
    {
        for (label g=1; g<nGroups; g++)
        {
            sigmaPhi += group_[g].isotopeSigmaPhi(i);
        }
    }

    return tmpSigmaPhi;
}


//- Update the buckling fields
/*!
    The method updateBuckling is called for each flux group.
*/
void Foam::diffusionFoam::fluxGroups::updateLeakage()
{
    forAll (group_, g)
    {
        group_[g].updateLeakage();
    }
}


//- Update the fission product isotopic concentrations
/*!
    Inform all broad energy groups that fission product isotopic concentrations have changed.
*/
void Foam::diffusionFoam::fluxGroups::updateIsotopes()
{
    forAll(group_, g)
    {
        group_[g].sigma().updateIsotopes();
    }
}


//- Scale the neutron fluxes by a given factor
void Foam::diffusionFoam::fluxGroups::scaleFlux(const scalar& factor)
{
    forAll (group_, g)
    {
        group_[g].scale(factor);
    }
}


//- Write object
bool Foam::diffusionFoam::fluxGroups::writeData(Ostream&) const
{
    return true;
}


// ************************************************************************* //
