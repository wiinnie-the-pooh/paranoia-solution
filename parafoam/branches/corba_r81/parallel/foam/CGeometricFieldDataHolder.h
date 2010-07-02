// VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
// Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#ifndef dev_CGeometricFieldDataHolder_h
#define dev_CGeometricFieldDataHolder_h


//---------------------------------------------------------------------------
#include "parallel/threading/dev/CSerializedDataHolderBase.h"


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
