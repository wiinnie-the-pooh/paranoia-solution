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
#ifndef TGenericObj_h
#define TGenericObj_h

#include "loki/Threads.h"


//---------------------------------------------------------------------------
/*
  Defines the intruisive counted base object 
*/

namespace parallel
{
  namespace base
  {
    struct TGenericObj;
    
    struct TGenericObj : Loki::ObjectLevelLockable< TGenericObj >
    {
      TGenericObj* Register();
      void Destroy();
      
      unsigned long GetRefCount();
      
    protected:
      TGenericObj();
      virtual ~TGenericObj() {}
      
    private:
      unsigned long m_ref_counter;
      
      TGenericObj( const TGenericObj& );  // Not implemented
      void operator = ( const TGenericObj& );   // Not implemented
    };
  }
}


//---------------------------------------------------------------------------
#endif
