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
#ifndef parallel_corba_SmartPtr_hh
#define parallel_corba_SmartPtr_hh


//---------------------------------------------------------------------------
#include "parallel/SmartPointer.hh"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace corba
  {
    //---------------------------------------------------------------------------
    template< class P >
    struct CORBAComparision
    {
      static P Equal( const P& theLeft, const P& theRight )
      {
        return strcmp( theLeft->IOR().in(), theRight->IOR().in() ) == 0;
      }
      
      static bool Less( const P& theLeft, const P& theRight )
      {
        return strcmp( theLeft->IOR().in(), theRight->IOR().in() ) < 0;
      }
    };


   //---------------------------------------------------------------------------
    template
    <
      typename T,
      template <class> class OwnershipPolicy = Loki::COMRefCounted,
      class ConversionPolicy = Loki::DisallowConversion,
      template <class> class CheckingPolicy = Loki::AssertCheck,
      template <class> class StoragePolicy = Loki::DefaultSPStorage,
      template<class> class ConstnessPolicy = LOKI_DEFAULT_CONSTNESS,
      template <class> class ComparisionPolicy = CORBAComparision
    >
    struct SmartPtrDef
    {
      typedef ::parallel::SmartPointer
      <
        T,
        OwnershipPolicy,
        ConversionPolicy,
        CheckingPolicy,
        StoragePolicy,
        ConstnessPolicy,
        ComparisionPolicy
      >
      type;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
