
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        fixedMaterialCrossSectionSet.H
    Class:
        fixedMaterialCrossSectionSet
    Description:
        Container class for fixed value material based macroscopic nuclear broad-group cross-sections
        and other broad-group nuclear data for a single broad energy group.
    SourceFiles:
        fixedMaterialCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#ifndef fixedMaterialCrossSectionSet_H
#define fixedMaterialCrossSectionSet_H

#include "crossSectionSet.H"
#include "IOList.H"
#include "DimensionedField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace diffusionFoam
{

// Class Forward Declarations
class fissionProducts;
class isotope;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace crossSections
{

/*---------------------------------------------------------------------------*\
                         Class crossSection Declaration
\*---------------------------------------------------------------------------*/
//- Container class for fixed value material based macroscopic nuclear broad-group cross-sections
//- and other broad-group nuclear data for a single broad energy group.*/
class fixedMaterialCrossSectionSet
:
    public crossSectionSet
{
protected:
    // Protected data

        //- Reference to material IDs
        IOList<label>& materialID_;

        //- Number of materials
        const label nMaterials_;

        //- Material-based mean neutron velocity
        IOList<scalar> v_byID_;
/*
        //- Material-based diffusion length
        IOList<scalar> D_byID_;
*/
        //- Material-based macroscopic transport cross-section
        IOList<scalar> Tr_byID_;

        //- Material-based macroscopic absorption cross-section
        IOList<scalar> A_byID_;

        //- Material-based macroscopic scattering cross-section
        PtrList<IOList<scalar> > S_byID_;

        //- Material-based macroscopic nu-fission cross-section
        IOList<scalar> nuF_byID_;

        //- Material-based macroscopic fission cross-section
        IOList<scalar> F_byID_;

        //- Material-based fission spectrum
        IOList<scalar> chi_byID_;

        //- Material-based fission product isotope microscopic cross-sections
        PtrList<IOList<scalar> > isotopeSigma_byID_;

        //- Anisotropic diffusion coefficient correction
        autoPtr<IOList<tensor> > kD_byID_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fixedMaterialCrossSectionSet(const fixedMaterialCrossSectionSet&);

        //- Disallow default bitwise assignment
        void operator=(const fixedMaterialCrossSectionSet&);

        // Test

            //- Check each list contains nMaterials values
            void checkFields();

        // Update

            //- set the contents of the field variables using the material-based values
            void setFields();

public:

    // Constructors

        //- Construct from dictionary
        fixedMaterialCrossSectionSet
        (
            const label index,
            const label nGroups,
            const label nMaterials,
            IOList<label>& materialID,
            const fissionProducts& products,
            const fvMesh& mesh,
            const dictionary& dict
        );


    // Destructor

        ~fixedMaterialCrossSectionSet();


    // Member Functions

        // Update

            //- Update crossSection values
            virtual void update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace crossSections
} // End namespace diffusionFoam
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
