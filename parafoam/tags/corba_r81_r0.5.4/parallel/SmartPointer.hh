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
#ifndef parallel_base_SmartPointer_hh
#define parallel_base_SmartPointer_hh


//---------------------------------------------------------------------------
#include <loki/SmartPtr.h>


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  template
  <
    typename T,
    template <class> class OwnershipPolicy,
    class ConversionPolicy,
    template <class> class CheckingPolicy,
    template <class> class StoragePolicy,
    template <class> class ConstnessPolicy,
    template <class> class ComparisionPolicy
  >
  struct SmartPointer : 
    Loki::SmartPtr< T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy, ConstnessPolicy > ,
    ComparisionPolicy< typename StoragePolicy< T >::InitPointerType >
  {
    typedef ConversionPolicy CP;
    
    typedef StoragePolicy< T > SP;
    typedef typename SP::StoredType StoredType;
    
  private:
    struct NeverMatched {};
    
#ifdef LOKI_SMARTPTR_CONVERSION_CONSTRUCTOR_POLICY
    typedef typename Select< CP::allow, const StoredType&, NeverMatched >::Result ImplicitArg;
    typedef typename Select< !CP::allow, const StoredType&, NeverMatched >::Result ExplicitArg;
#else
    typedef const StoredType& ImplicitArg;
    typedef typename Loki::Select< false, const StoredType&, NeverMatched >::Result ExplicitArg;
#endif

  public:
    typedef ComparisionPolicy< typename StoragePolicy< T >::StoredType > CMP;
    
    typedef Loki::SmartPtr< T,
                            OwnershipPolicy,
                            ConversionPolicy,
                            CheckingPolicy,
                            StoragePolicy,
                            ConstnessPolicy > TSmartPtr;
    
    SmartPointer()
    {}
    
    explicit
    SmartPointer( ExplicitArg p )
      : TSmartPtr( p )
    {}
    
    SmartPointer( ImplicitArg p )
      : TSmartPtr( p )
    {}
    
    SmartPointer( typename TSmartPtr::CopyArg& rhs )
      : TSmartPtr( rhs )
    {}
    
    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    SmartPointer( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs )
      : TSmartPtr( Loki::SmartPtr< T1, OP1, CP1, KP1, SP1, CNP1 > ( rhs ) )
    {}

    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    SmartPointer( SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs )
      : TSmartPtr( Loki::SmartPtr< T1, OP1, CP1, KP1, SP1, CNP1 > ( rhs ) )
    {}
    
    SmartPointer( Loki::RefToValue< SmartPointer > rhs )
      : TSmartPtr( rhs )
    {}


    //---------------------------------------------------------------------------
    typedef typename ConstnessPolicy< typename SP::PointerType >::Type ConstPointerType;

    ConstPointerType operator -> () const
    {
      typedef CheckingPolicy< typename StoragePolicy< T >::StoredType > KP;
      KP::OnDereference( GetImplRef( *this ) );
      
      return SP::operator->();
    }


    //---------------------------------------------------------------------------
    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator == ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return CMP::Equal( GetImpl( *this ), GetImpl( rhs ) ); 
    }
    
    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator != ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return !CMP::Equal( GetImpl( *this ), GetImpl( rhs ) ); 
    }
    
    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator < ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return CMP::Less( GetImpl( *this ), GetImpl( rhs ) ); 
    }
    
    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator > ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return CMP::Less( GetImpl( rhs ), GetImpl( *this ) ); 
    }

    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator <= ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return !CMP::Less( GetImpl( rhs ), GetImpl( *this ) ); 
    }

    template
    <
      typename T1,
      template <class> class OP1,
      class CP1,
      template <class> class KP1,
      template <class> class SP1,
      template <class> class CNP1,
      template <class> class CMP1
    >
    bool operator >= ( const SmartPointer< T1, OP1, CP1, KP1, SP1, CNP1, CMP1 >& rhs ) const
    {
      return !CMP::Less( GetImpl( *this ), GetImpl( rhs ) ); 
    }
  };
    

  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace std
{
  template
  <
    typename T,
    template <class> class OP,
    class CP,
    template <class> class KP,
    template <class> class SP,
    template <class> class CNP,
    template <class> class CMP
  >
  struct less< parallel::SmartPointer< T, OP, CP, KP, SP, CNP, CMP > >
    : public binary_function< parallel::SmartPointer< T, OP, CP, KP, SP, CNP, CMP >,
                              parallel::SmartPointer< T, OP, CP, KP, SP, CNP, CMP >, bool >
  {
    bool operator()( const parallel::SmartPointer< T, OP, CP, KP, SP, CNP, CMP >& lhs,
                     const parallel::SmartPointer< T, OP, CP, KP, SP, CNP, CMP >& rhs ) const
    { 
      return lhs.operator < ( rhs ); 
    }
  };
}


//---------------------------------------------------------------------------
#endif
