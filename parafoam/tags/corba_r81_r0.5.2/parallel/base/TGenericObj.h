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
