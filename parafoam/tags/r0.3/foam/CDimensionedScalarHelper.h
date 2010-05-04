// Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License. 
// 
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details. 
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#ifndef dev_CDimensionedScalarHelper_h
#define dev_CDimensionedScalarHelper_h


//---------------------------------------------------------------------------
#include "parallel/dev/CSimpleValueHelper.h"

#include <dimensionedScalar.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template<>
    struct CSimpleValueHelper< Foam::dimensionedScalar > : CSimpleValueHelperBase< Foam::dimensionedScalar >
    {
      CSimpleValueHelper( const TValue& the_value = Foam::dimensionedScalar( 0.0 ) )
        : CSimpleValueHelperBase< Foam::dimensionedScalar >( the_value )
      {}

      CSimpleValueHelper( const CSimpleValueHelper& the_value_helper )
        : CSimpleValueHelperBase< Foam::dimensionedScalar >( the_value_helper.value )
      {}
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
