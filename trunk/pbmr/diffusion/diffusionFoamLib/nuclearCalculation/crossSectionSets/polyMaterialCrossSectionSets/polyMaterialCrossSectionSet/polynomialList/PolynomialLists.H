
/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code may be reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        PolynomialLists.H
    SourceFiles:
        PolynomialLists.C
-------------------------------------------------------------------------------------------------*/

#ifndef PolynomialLists_H
#define PolynomialLists_H

#include "PolynomialList.H"
#include "polynomial.H"
#include "tensorPolynomial.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

typedef PolynomialList<polynomial> polynomialList;
typedef PolynomialList<tensorPolynomial> tensorPolynomialList;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

//-----------------------------------------------------------------------------------------------//
