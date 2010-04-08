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
#ifndef dev_CGeometricFieldDataHolder_h
#define dev_CGeometricFieldDataHolder_h


//---------------------------------------------------------------------------
#include "parallel/dev/CSerializedDataHolderBase.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    template< class SerializedValueHelperType >
    struct CGeometricFieldDataHolder : dev::CSerializedDataHolderBase< SerializedValueHelperType >
    {
      typedef SerializedValueHelperType TValueHelper;
      typedef typename SerializedValueHelperType::TMesh TMesh;

      CGeometricFieldDataHolder( const SerializedValueHelperType& the_value_holder )
        : dev::CSerializedDataHolderBase< SerializedValueHelperType >( the_value_holder )
      {}
      
      SerializedValueHelperType value( const TMesh& the_mesh )
      {
        SerializedValueHelperType a_value_helper( the_mesh );
        
        a_value_helper.restore_data( * this );

        return a_value_helper;
      }
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
