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
#ifndef TSmartPtr_h
#define TSmartPtr_h


//---------------------------------------------------------------------------
#include "parallel/base/TGenericObj.h"

#include <loki/SmartPtr.h>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace base
  {
    //---------------------------------------------------------------------------
    template< class P >
    struct TOwnershipPolicy
    {
      TOwnershipPolicy()
      {}
      
      template< class U >
      TOwnershipPolicy( const TOwnershipPolicy< U >& )
      {}

      static P Clone( const P& val )
      {
        if ( val != 0 )
          val->Register();
        
        return val;
      }

      static bool Release( const P& val )
      {
        if ( val != 0 )
          val->Destroy();
        
        return false;
      }
      
      enum { destructiveCopy = false };
      
      static void Swap( TOwnershipPolicy& )
      {}
    };


    //---------------------------------------------------------------------------
    template
    <
      typename T,
      template <class> class OwnershipPolicy = TOwnershipPolicy,
      class ConversionPolicy = Loki::DisallowConversion,
      template <class> class CheckingPolicy = Loki::AssertCheck,
      template <class> class StoragePolicy = Loki::DefaultSPStorage,
      template<class> class ConstnessPolicy = LOKI_DEFAULT_CONSTNESS
    >
    struct SmartPtrDef
    {
      typedef Loki::SmartPtr
      <
        T,
        OwnershipPolicy,
        ConversionPolicy,
        CheckingPolicy,
        StoragePolicy,
        ConstnessPolicy
      >
      type;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
