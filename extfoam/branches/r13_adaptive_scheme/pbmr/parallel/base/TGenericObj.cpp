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
