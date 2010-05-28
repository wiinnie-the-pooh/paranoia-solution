
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        polyMaterialCrossSectionSet.H
    Class:
        polyMaterialCrossSectionSet
    Description:
        Container class for polynomial expansion material based macroscopic nuclear
        broad-group cross-sections and other broad-group nuclear data for a single
        broad energy group.
    SourceFiles:
        polyMaterialCrossSectionSet.C
-------------------------------------------------------------------------------------------------*/

#ifndef fixedMaterialCrossSectionSet_H
#define fixedMaterialCrossSectionSet_H

#include "crossSectionSet.H"
#include "PolynomialLists.H"
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
//- Container class for polynomial expansion material based macroscopic nuclear
//- broad-group cross-sections and other broad-group nuclear data for a single
//- broad energy group.
//- TODO Should be written using indirect lists for each material rather than
//- a single material map to improve performance accessing polynomial objects
class polyMaterialCrossSectionSet
:
    public crossSectionSet
{
protected:
    // Protected data

        //- Reference to material IDs
        const IOList<label>& materialID_;

        //- Number of materials
        const label nMaterials_;

        //- Constant reference to names of reference fields
        const List<word>& refFieldName_;

        //- Constant references to referenced fields
        const List<const Field<scalar>*>& refFieldPtr_;

        //- Material-based reference values
        const List<List<scalar> >& refValue_;

        //- Material-based mean neutron velocity coefficients
        polynomialList v_byID_;

//        //- Material-based diffusion length coefficients
//        polynomialList D_byID_;

        //- Material-based macroscopic transport cross-section coefficients
        tensorPolynomialList Tr_byID_;

        //- Material-based macroscopic absorption cross-section coefficients
        polynomialList A_byID_;

        //- Material-based macroscopic scattering cross-section coefficients
        PtrList<polynomialList> S_byID_;

        //- Material-based macroscopic nu-fission cross-section coefficients
        polynomialList nuF_byID_;

        //- Material-based macroscopic fission cross-section coefficients
        polynomialList F_byID_;

        //- Material-based fission spectrum coefficients
        polynomialList chi_byID_;

        //- Material-based fission product isotope microscopic cross-sections coefficients
        PtrList<polynomialList> isotopeSigma_byID_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        polyMaterialCrossSectionSet(const polyMaterialCrossSectionSet&);

        //- Disallow default bitwise assignment

        // Update

            //- Set the contents of the field variables using the material-based values
            void setFields();

public:

    // Constructors

        //- Construct from dictionary
        polyMaterialCrossSectionSet
        (
            const label index,
            const label nGroups,
            const label nMaterials,
            const IOList<label>& materialID,
            const List<word>& refFieldName,
            const List<const Field<scalar>*>& refFieldPtr,
            const List<List<scalar> >& refValue,
            const fissionProducts& products,
            const fvMesh& mesh,
            const dictionary& dict
        );


    // Destructor

        ~polyMaterialCrossSectionSet();


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
