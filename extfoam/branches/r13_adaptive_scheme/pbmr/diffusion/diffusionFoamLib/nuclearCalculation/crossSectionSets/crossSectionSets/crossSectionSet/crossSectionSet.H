
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        crossSectionSet.H
    Class:
        crossSectionSet
    Description:
        Virtual base container class for macroscopic nuclear broad-group cross-sections and
        other broad-group nuclear data for a single broad energy group.
    SourceFiles:
        crossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#ifndef crossSectionSet_H
#define crossSectionSet_H

#include "fvMesh.H"
#include "volFields.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class Forward Declarations
class fissionProducts;
class isotope;

/*---------------------------------------------------------------------------*\
                         Class crossSection Declaration
\*---------------------------------------------------------------------------*/
//- Virtual base container class for macroscopic nuclear broad-group
//- cross-sections and other broad-group nuclear data for a single broad
//- energy group.
class crossSectionSet
{
protected:
    // Protected data
        //- Reference to mesh
        const fvMesh& mesh_;

        //- Group index
        const label index_;

        //- Inverse mean neutron velocity field
        autoPtr<volScalarField> v_;

        //- Macroscopic transport cross-section
        autoPtr<volScalarField> Tr_;

        //- Macroscopic absorption cross-section field
        autoPtr<volScalarField> A_;

        //- Total macroscopic absorption cross-section field,
        //- including absorbing isotopes
        autoPtr<volScalarField> Atot_;

        //- Macroscopic scattering cross-sections
        PtrList<volScalarField> S_;

        //- Macroscopic nu-fission cross-section
        autoPtr<volScalarField> nuF_;

        //- Macroscopic fission cross-section
        autoPtr<volScalarField> F_;

        //- Fission spectrum
        autoPtr<volScalarField> chi_;

        //- Isotope specific neutron absorption (microscopic cross-sections)
        PtrList<volScalarField> isotopeSigma_;

        //- Reference to list of isotopes
        const PtrList<isotope>& isotope_;

        //- Anisotropic diffusion correction tensor
        //- \f$ \vec{D} = \frac{\vec{k_D}}{3\Sigma_{tr}} \f$
        autoPtr<volTensorField> kD_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        crossSectionSet(const crossSectionSet&);

        //- Disallow default bitwise assignment
        void operator=(const crossSectionSet&);


public:

    // Static data

        // Declare name of the class and its debug switch
        ClassName("crossSectionSet");

    // Constructors

        //- Construct from dictionary
        crossSectionSet
        (
            const label index,
            const label nGroups,
            const fissionProducts& products,
            const fvMesh& mesh,
            const dictionary& dict,
            const IOobject::readOption r,
            const IOobject::writeOption w
        );

    // Destructor

        virtual ~crossSectionSet()
        {}


    // Member Functions

        // Access

            //- Return the inverse mean neutron velocity field \f$ \frac{1}{v_g} \f$ [s/m]
            inline const volScalarField& v() const
            {
                return v_();
            }

            //- Return macroscopic transport cross-section \f$ \Sigma_{tr,g} \f$ [m]
            inline const volScalarField& Tr() const
            {
                return Tr_();
            }

            //- Return macroscopic absorption cross-section \f$ \Sigma_{a0,g} \f$ [/m]
            //- Excludes the absorbing isotopes xenon,samarium, etc.
            inline const volScalarField& A() const
            {
                return A_();
            }

            //- Return the total macroscopic absorption cross-section field
            //- \f$ \Sigma_{a0,g} + \Sigma_{X,g} \f$ [/m]
            //- Includes the the absorbing isotopes xenon,samarium, etc.
            inline const volScalarField& Atot() const
            {
                return Atot_();
            }

            //- Return macroscopic scattering cross-section list \f$ \Sigma_S^{g \rightarrow g'} \f$ [/m]
            inline const PtrList<volScalarField>& S() const
            {
                return S_;
            }

            //- Return the macroscopic nu-fission cross-section field \f$ \nu\Sigma_{f,g} \f$ [/m]
            inline const volScalarField& nuF() const
            {
                return nuF_();
            }

            //- Return the macroscopic fission cross-section field \f$ \Sigma_{f,g} \f$ [/m]
            inline const volScalarField& F() const
            {
                return F_();
            }

            //- Return the fission spectrum field \f$ \chi_g \f$
            inline const volScalarField& chi() const
            {
                return chi_();
            }

            //- Return the microscopic absorption cross-section for a given isotope \f$ \sigma_{a,g} \f$ [m^2]
            inline const volScalarField& isotopeSigma(const label i) const
            {
                return isotopeSigma_[i];
            }

            //- Return the number of isotopes
            inline label nIsotopes() const
            {
                return isotope_.size();
            }

            //- Return true if diffusion term is anisotropic
            inline bool anisotropicD() const
            {
                return kD_.valid();
            }

            //- Return the anisotropic diffusion correction tensorA()
            //- \f$ \vec{k_D} \f$, where \f$ \vec{D}=\vec{k_D} D \f$
            const volTensorField& kD() const;

            //- Return the anisotropic diffusion correction tensorA()
            //- \f$ \vec{k_D} \f$, where \f$ \vec{D}=\vec{k_D} D \f$.
            //- Create if not available
            const volTensorField& kD();

        // Update

            //- Update the removal cross-sections
            void updateRemoval();

            //- Update the boundary fields
            void updateBoundaryFields();

            //- Update total macroscopic absorption cross-section, including absorber isotopes
            void updateIsotopes();

            //- Update crossSection values
            virtual void update()
            {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusionFoam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
