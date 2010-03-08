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
#ifndef foam_CVolScalarFieldPort_h
#define foam_CVolScalarFieldPort_h


//---------------------------------------------------------------------------
#include "parallel/dev/TPort.h"

#include "parallel/dev/TTask.h"

#include "parallel/foam/CTmpDataHolderBase.h"

#include <volFields.H>

#include <loki/SmartPtr.h>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    struct CVolScalarFieldPort : dev::TPort
    {
      typedef Foam::volScalarField THold;

      PARALLEL_DERIVED_PORT_DEF( CVolScalarFieldPort );
        
      struct TDataHolder : base::TDataHolder, CTmpDataHolderBase< THold >
      {
        TDataHolder()
        {}

        TDataHolder( const TValueHelper& the_value_helper )
          : CTmpDataHolderBase< THold >( the_value_helper )
        {}
      };

      typedef TDataHolder::TValueHelper TValueHelper;
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
