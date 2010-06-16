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
#ifndef dev_CSerializedDataHolderBase_h
#define dev_CSerializedDataHolderBase_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class SerializedValueHelperType >
    struct CSerializedDataHolderBase : base::TDataHolder
    {
      typedef SerializedValueHelperType TValueHelper;

      CSerializedDataHolderBase( const SerializedValueHelperType& the_value_holder )
      {
        the_value_holder.save_data( *this );
      }
      
      CSerializedDataHolderBase& operator = ( const std::string& the_serialized_data )
      {
        this->serialized_data = the_serialized_data;
        
        return *this;
      }

      const std::string& operator () () const
      {
        return this->serialized_data;
      }

      std::string& operator () ()
      {
        return this->serialized_data;
      }

    protected:
      std::string serialized_data;
    };

    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
