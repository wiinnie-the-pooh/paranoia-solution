/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        PolynomialListI.H
-------------------------------------------------------------------------------------------------*/

namespace Foam
{

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
const T& PolynomialList<T>::operator[](const label i) const
{
    if (!PtrList<T>::operator()(i))
    {
        FatalErrorIn("PolynomialList::operator[] const")
            << "Undefined for material " << i
            << abort(FatalError);
    }

    return PtrList<T>::operator[](i);
}


template<class T>
T& PolynomialList<T>::operator[](const label i)
{
    if (!PtrList<T>::operator()(i))
    {
        FatalErrorIn("PolynomialList::operator[] const")
            << "Undefined for material " << i
            << abort(FatalError);
    }

    return PtrList<T>::operator[](i);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//-----------------------------------------------------------------------------------------------//
