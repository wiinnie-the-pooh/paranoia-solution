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
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#ifndef dev_CSerializedDataHolder_h
#define dev_CSerializedDataHolder_h


//---------------------------------------------------------------------------
#include "parallel/dev/CSerializedDataHolderBase.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class SerializedValueHelperType >
    struct CSerializedDataHolder : CSerializedDataHolderBase< SerializedValueHelperType >
    {
      typedef SerializedValueHelperType TValueHelper;

      CSerializedDataHolder( const SerializedValueHelperType& the_value_holder )
        : CSerializedDataHolderBase< SerializedValueHelperType >( the_value_holder )
      {}
      
      SerializedValueHelperType value()
      {
        SerializedValueHelperType a_value_helper;
        
        a_value_helper.restore_data( * this );

        return a_value_helper;
      }
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
