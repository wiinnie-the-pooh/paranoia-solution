
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        PolynomialList.H
    Class:
        PolynomialList
    Description:
        List of polynomials with dictionary initialize
    SourceFiles:
        PolynomialList.C
-------------------------------------------------------------------------------------------------*/

#ifndef PolynomialList_H
#define PolynomialList_H

#include "dictionaryEntry.H"
#include "PtrList.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class polynomial Declaration
\*---------------------------------------------------------------------------*/
//-    List of polynomials with dictionary initialize
template<class Type>
class PolynomialList
:
    public PtrList<Type>
{
protected:
    // Protected data

        //- Polynomial coefficients
        PtrList<Type> list_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        PolynomialList(const PolynomialList&);

        //- Disallow default bitwise assignment
        void operator=(const PolynomialList&);

public:

    //- Runtime type information
    TypeName("PolynomialList");

    // Constructors

        //- Construct from dictionary
        PolynomialList
        (
            const word& dictName,
            const fvMesh& mesh,
            const label nMaterials,
            const List<word>& refFieldName
        );


    // Destructor

        virtual ~PolynomialList();


    // Member Functions

        //- Redefined PtrList operator for better error messaging
        inline const Type& operator[](const label) const;

        //- Redefined PtrList operator for better error messaging
        inline Type& operator[](const label);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "PolynomialListI.H"

#ifdef NoRepository
#   include "PolynomialList.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
