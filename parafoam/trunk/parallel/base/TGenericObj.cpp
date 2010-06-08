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
#include "parallel/base/TGenericObj.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace base
  {
    //---------------------------------------------------------------------------
    TGenericObj::TGenericObj()
    {
      Lock a_lock( *this );
      m_ref_counter = 1;
    }

    //---------------------------------------------------------------------------
    TGenericObj* TGenericObj::Register()
    {
      Lock a_lock( *this );
      ++m_ref_counter;
      
      return this;
    }

    //---------------------------------------------------------------------------
    void TGenericObj::Destroy()
    {
      bool an_is_zero = false;
      {
        Lock a_lock( *this );
        an_is_zero = ( --m_ref_counter == 0 );
      }
      
      if ( an_is_zero )
        delete this;
    }

    //---------------------------------------------------------------------------
    unsigned long TGenericObj::GetRefCount()
    {
      return m_ref_counter;
    }

    //---------------------------------------------------------------------------
  }
}

//---------------------------------------------------------------------------
