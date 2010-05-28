/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        extrapolatedLengthFvPatchField.C
-------------------------------------------------------------------------------------------------*/

#include "extrapolatedLengthFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
extrapolatedLengthFvPatchField<Type>::extrapolatedLengthFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(p, iF),
    length_(p.size(), pTraits<scalar>::zero),
    phiName_("phi")
{
}


template<class Type>
extrapolatedLengthFvPatchField<Type>::extrapolatedLengthFvPatchField
(
    const extrapolatedLengthFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<Type>(ptf, p, iF, mapper),
    length_(ptf.length_, mapper),
	phiName_(ptf.phiName_)
{
}


template<class Type>
extrapolatedLengthFvPatchField<Type>::extrapolatedLengthFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<Type>(p, iF),
    length_("length", dict, p.size()),
    phiName_(word(dict.lookup("phi")))
{
}


template<class Type>
extrapolatedLengthFvPatchField<Type>::extrapolatedLengthFvPatchField
(
    const extrapolatedLengthFvPatchField<Type>& ptf
)
:
    fvPatchField<Type>(ptf),
    length_(ptf.length_),
    phiName_(ptf.phiName_)
{
}


template<class Type>
extrapolatedLengthFvPatchField<Type>::extrapolatedLengthFvPatchField
(
    const extrapolatedLengthFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(ptf, iF),
    length_(ptf.length_),
    phiName_(ptf.phiName_)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the coefficients associated with the patch field
/*!
    The associated field is assumed to be the diffusion length.
    The extrapolated length is calculated according to:
    \f[
        \lambda_{extrap} = \frac{0.7104}{\Sigma_{tr}}
    \f]
*/
template<class Type>
void extrapolatedLengthFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

	const fvPatchField<scalar>& phi_ = this->patch().lookupPatchField
	(
		phiName_,
		reinterpret_cast<const volScalarField*>(NULL),
		reinterpret_cast<const scalar*>(NULL)
	);

	// Extrapolated length = 0.71 / Sigma_tr
    length_ = 0.7104/phi_;
//    length_ = 0.6666667/phi_;

    fvPatchField<Type>::updateCoeffs();
}


//- Return gradient at boundary.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > extrapolatedLengthFvPatchField<scalar>::snGrad() const
{
    return -this->patchInternalField()*this->patch().deltaCoeffs()/(scalar(1.0)+length_*this->patch().deltaCoeffs());
}


//- Return gradient at boundary.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > extrapolatedLengthFvPatchField<Type>::snGrad() const
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "snGrad()"
    );

    return Field<Type>::null();
}


//- Evaluate the field on the patch.
/*!
    Implementation for the scalar case.
*/
template<>
void extrapolatedLengthFvPatchField<scalar>::evaluate()
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    Field<scalar>::operator=
    (
        this->patchInternalField()*length_/(scalar(1.0)/this->patch().deltaCoeffs()+length_)
    );

    fvPatchField<scalar>::evaluate();
}


//- Evaluate the field on the patch.
/*!
    Not implemented for the general case.
*/
template<class Type>
void extrapolatedLengthFvPatchField<Type>::evaluate()
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "evaluate"
    );
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > extrapolatedLengthFvPatchField<scalar>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    return tmp<Field<scalar> >
	(
		new Field<scalar>
		(
			length_/(scalar(1.0)/this->patch().deltaCoeffs()+length_)
		)
	);
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > extrapolatedLengthFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "valueInternalCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix source coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > extrapolatedLengthFvPatchField<scalar>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    return tmp<Field<scalar> >
    (
        new Field<scalar>(this->size(), pTraits<scalar>::zero)
    );
}


//- Return the matrix source coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > extrapolatedLengthFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "valueBoundaryCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > extrapolatedLengthFvPatchField<scalar>::gradientInternalCoeffs() const
{
    return tmp<Field<scalar> >
	(
		new Field<scalar>
		(
			-this->patch().deltaCoeffs()/(scalar(1.0)+length_*this->patch().deltaCoeffs())
		)
	);
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > extrapolatedLengthFvPatchField<Type>::gradientInternalCoeffs() const
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "gradientInternalCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix source coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > extrapolatedLengthFvPatchField<scalar>::gradientBoundaryCoeffs() const
{
    return tmp<Field<scalar> >
    (
        new Field<scalar>(this->size(), pTraits<scalar>::zero)
    );
}


//- Return the matrix source coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > extrapolatedLengthFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    notImplemented
    (
        "extrapolatedLengthFvPatchField<Type>::"
        "gradientBoundaryCoeffs()"
    );

    return Field<Type>::null();
}


//- Write to an output stream
template<class Type>
void extrapolatedLengthFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    if (phiName_ != "phi")
    {
        os.writeKeyword("phi")
            << phiName_ << token::END_STATEMENT << nl;
    }
    this->length_.writeEntry("length", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
