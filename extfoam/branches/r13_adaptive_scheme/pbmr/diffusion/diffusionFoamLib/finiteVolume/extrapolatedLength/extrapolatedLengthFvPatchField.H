/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        extrapolatedLengthFvPatchField.H
    Class:
        extrapolatedLengthFvPatchField
    Description:
        Extrapolated length boundary field for scalar neutron flux
    SourceFiles:
        extrapolatedLengthFvPatchField.C
-------------------------------------------------------------------------------------------------*/


#ifndef extrapolatedLengthFvPatchField_H
#define extrapolatedLengthFvPatchField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class extrapolatedLengthFvPatch Declaration
\*---------------------------------------------------------------------------*/
//- Extrapolated length boundary field for scalar neutron flux
template<class Type>
class extrapolatedLengthFvPatchField
:
    public fvPatchField<Type>
{
    // Private data

        //- Extrapolation distance field [m]
        scalarField length_;

        //- Name of the field on which to look up diffusion length values
        word phiName_;

    public:

    //- Runtime type information
    TypeName("extrapolatedLength");


    // Constructors

        //- Construct from patch and internal field
        extrapolatedLengthFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        extrapolatedLengthFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given extrapolatedLengthFvPatchField<Type> onto a new patch
        extrapolatedLengthFvPatchField
        (
            const extrapolatedLengthFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );


        //- Construct as copy
        extrapolatedLengthFvPatchField
        (
            const extrapolatedLengthFvPatchField<Type>&
        );


        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new extrapolatedLengthFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        extrapolatedLengthFvPatchField
        (
            const extrapolatedLengthFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );


        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new extrapolatedLengthFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        //- Return a non-constant reference to the extrapolation distance
        virtual scalarField& length()
        {
            return length_;
        }

        //- Return a reference to the extrapolation distance
        virtual const scalarField& length() const
        {
            return length_;
        }

    // Evaluation functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Return gradient at boundary
        virtual tmp<Field<Type> > snGrad() const;

        //- Evaluate the patch field
        virtual void evaluate();

        //- Return the matrix diagonal coefficients corresponding to the evaluation of the value of this patchField
        virtual tmp<Field<Type> > valueInternalCoeffs
        (
            const tmp<scalarField>&
        ) const;

        //- Return the matrix source coefficients corresponding to the evaluation of the value of this patchField,
        virtual tmp<Field<Type> > valueBoundaryCoeffs
        (
            const tmp<scalarField>&
        ) const;

        //- Return the matrix diagonal coefficients corresponding to the evaluation of the gradient of this patchField
        virtual tmp<Field<Type> > gradientInternalCoeffs() const;

        //- Return the matrix source coefficients corresponding to the evaluation of the gradient of this patchField
        virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;


        //- Write to an output stream
        virtual void write(Ostream&) const;


    // Member operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const fvPatchField<Type>&) {}
        virtual void operator+=(const fvPatchField<Type>&) {}
        virtual void operator-=(const fvPatchField<Type>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<Type>&) {}
        virtual void operator-=(const Field<Type>&) {}

        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const Type&) {}
        virtual void operator+=(const Type&) {}
        virtual void operator-=(const Type&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "extrapolatedLengthFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
