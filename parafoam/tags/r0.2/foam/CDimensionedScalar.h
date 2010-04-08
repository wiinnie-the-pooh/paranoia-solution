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
#ifndef foam_CDimensionedScalar_h
#define foam_CDimensionedScalar_h


//---------------------------------------------------------------------------
#include "parallel/dev/TPort.h"

#include "parallel/dev/TTask.h"

#include "parallel/dev/CSimpleDataHolderBase.h"

#include "parallel/foam/CDimensionedScalarHelper.h"

#include <loki/SmartPtr.h>


//---------------------------------------------------------------------------
namespace parallel 
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    struct CDimensionedScalar : dev::TPort
    {
      typedef Foam::dimensionedScalar THold;

      CDimensionedScalar( const std::string& theName, dev::TTask& theTask )
        : dev::TPort( theName, theTask )
      {}
      
      struct TDataHolder : base::TDataHolder, dev::CSimpleDataHolderBase< THold >
      {
        TDataHolder( const TValueHelper& the_value_helper )
          : dev::CSimpleDataHolderBase< THold >( the_value_helper )
        {}
      };

      typedef TDataHolder::TValueHelper TValueHelper;
    };
    

    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
