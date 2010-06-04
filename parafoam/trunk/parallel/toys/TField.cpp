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
#include "parallel/toys/TField.h"

#include <string.h>
#include <iostream>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    TField::TField( const std::string& theName, TData theData, int theSize )
      : m_Name( theName )
    {
      m_Data = malloc( theSize );
      memcpy( m_Data, theData, theSize );
    }

    TField::~TField()
    {
      //std::cout << "~TField()" << std::endl;
      free( m_Data );
    }

    std::string TField::get_name()
    {
      return m_Name;
    }
    
    TData TField::get_raw_data()
    {
      return m_Data;
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
