/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        albedoFvPatchField.C
-------------------------------------------------------------------------------------------------*/

#include "albedoFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
albedoFvPatchField<Type>::albedoFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(p, iF),
    albedo_(p.size(), pTraits<scalar>::one),
    internalCoeffs_(p.size(), 0),
    phiName_("phi")
{
}


template<class Type>
albedoFvPatchField<Type>::albedoFvPatchField
(
    const albedoFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<Type>(ptf, p, iF, mapper),
    albedo_(ptf.albedo_, mapper),
    internalCoeffs_(p.size(), 0),
	phiName_(ptf.phiName_)
{
}


template<class Type>
albedoFvPatchField<Type>::albedoFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<Type>(p, iF),
    albedo_("albedo", dict, p.size()),
    internalCoeffs_(p.size(), 0),
    phiName_(word(dict.lookup("phi")))
{
}


template<class Type>
albedoFvPatchField<Type>::albedoFvPatchField
(
    const albedoFvPatchField<Type>& ptf
)
:
    fvPatchField<Type>(ptf),
    albedo_(ptf.albedo_),
    internalCoeffs_(ptf.internalCoeffs_),
    phiName_(ptf.phiName_)
{
}


template<class Type>
albedoFvPatchField<Type>::albedoFvPatchField
(
    const albedoFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(ptf, iF),
    albedo_(ptf.albedo_),
    internalCoeffs_(ptf.internalCoeffs_),
    phiName_(ptf.phiName_)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Update the coefficients associated with the patch field
template<class Type>
void albedoFvPatchField<Type>::updateCoeffs()
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

    const scalarField& delta = this->patch().deltaCoeffs();

    internalCoeffs_ = delta/(delta + 1.5*phi_*(1.0-albedo_)/(1.0+albedo_));

    fvPatchField<Type>::updateCoeffs();
}


//- Return gradient at boundary.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > albedoFvPatchField<scalar>::snGrad() const
{
    return this->patchInternalField()*this->patch().deltaCoeffs()*(internalCoeffs_-1.0);
}


//- Return gradient at boundary.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > albedoFvPatchField<Type>::snGrad() const
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "snGrad()"
    );

    return Field<Type>::null();
}


//- Evaluate the field on the patch.
/*!
    Implementation for the scalar case.
*/
template<>
void albedoFvPatchField<scalar>::evaluate()
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    Field<scalar>::operator=
    (
        this->patchInternalField()*internalCoeffs_
    );

    fvPatchField<scalar>::evaluate();
}


//- Evaluate the field on the patch.
/*!
    Not implemented for the general case.
*/
template<class Type>
void albedoFvPatchField<Type>::evaluate()
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "evaluate"
    );
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > albedoFvPatchField<scalar>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    return tmp<Field<scalar> >
	(
		new Field<scalar>
		(
            internalCoeffs_
		)
	);
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > albedoFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "valueInternalCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix source coefficients corresponding to the evaluation of the value of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > albedoFvPatchField<scalar>::valueBoundaryCoeffs
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
tmp<Field<Type> > albedoFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "valueBoundaryCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > albedoFvPatchField<scalar>::gradientInternalCoeffs() const
{
    return tmp<Field<scalar> >
	(
		new Field<scalar>
		(
			this->patch().deltaCoeffs()*(internalCoeffs_-1.0)
		)
	);
}


//- Return the matrix diagonal coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Not implemented for the general case.
*/
template<class Type>
tmp<Field<Type> > albedoFvPatchField<Type>::gradientInternalCoeffs() const
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "gradientInternalCoeffs()"
    );

    return Field<Type>::null();
}


//- Return the matrix source coefficients corresponding to the evaluation of the gradient of this patchField.
/*!
    Implementation for the scalar case.
*/
template<>
tmp<Field<scalar> > albedoFvPatchField<scalar>::gradientBoundaryCoeffs() const
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
tmp<Field<Type> > albedoFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    notImplemented
    (
        "albedoFvPatchField<Type>::"
        "gradientBoundaryCoeffs()"
    );

    return Field<Type>::null();
}


//- Write to an output stream
template<class Type>
void albedoFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    if (phiName_ != "phi")
    {
        os.writeKeyword("phi")
            << phiName_ << token::END_STATEMENT << nl;
    }
    this->albedo_.writeEntry("albedo", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
