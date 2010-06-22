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

#include <CORBA.h>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace corba
  {
    //---------------------------------------------------------------------------
    template < class T >
    class CORBAStorage
    {
    public:
      typedef T StoredType;    // the type of the pointee_ object
      typedef T InitPointerType; /// type used to declare OwnershipPolicy type.
      typedef T PointerType;   // type returned by operator->
      typedef T& ReferenceType; // type returned by operator*

      CORBAStorage() : pointee_( Default() )
      {}

      // The storage policy doesn't initialize the stored pointer
      //     which will be initialized by the OwnershipPolicy's Clone fn
      CORBAStorage( const CORBAStorage& ) 
	: pointee_( Default() )
      {}

      template < class U >
      CORBAStorage( const CORBAStorage< U >& ) 
	: pointee_( Default() )
      {}

      CORBAStorage( const StoredType& p ) 
	: pointee_( p ) 
      {}

      PointerType operator->() const 
      { 
	return pointee_; 
      }

      ReferenceType operator*() const 
      {
	return pointee_; 
      }

      void Swap( CORBAStorage& rhs )
      {
	std::swap( pointee_, rhs.pointee_ ); 
      }

      // Accessors
      template < class F >
      friend typename CORBAStorage< F >::PointerType GetImpl( const CORBAStorage< F >& sp );

      template < class F >
      friend const typename CORBAStorage< F >::StoredType& GetImplRef( const CORBAStorage< F >& sp );

      template < class F >
      friend typename CORBAStorage< F >::StoredType& GetImplRef( CORBAStorage<F>& sp );

    protected:
      // Destroys the data stored
      // (Destruction might be taken over by the OwnershipPolicy)
      //
      // If your compiler gives you a warning in this area while
      // compiling the tests, it is on purpose, please ignore it.
      void Destroy()
      {
	//delete pointee_;
      }

      // Default value to initialize the pointer
      static StoredType Default()
      { 
	return StoredType(); 
      }

    private:
        // Data
        StoredType pointee_;
    };
    
    
    //---------------------------------------------------------------------------
    template < class T >
    inline typename CORBAStorage< T >::PointerType GetImpl( const CORBAStorage< T >& sp )
    { 
      return sp.pointee_; 
    }

    template < class T >
    inline const typename CORBAStorage< T >::StoredType& GetImplRef( const CORBAStorage< T >& sp )
    {
      return sp.pointee_; 
    }

    template < class T >
    inline typename CORBAStorage< T >::StoredType& GetImplRef( CORBAStorage< T >& sp )
    {
      return sp.pointee_; 
    }


    //---------------------------------------------------------------------------
    template <class P>
    struct CORBARefCounted
    {
      CORBARefCounted()
      {}

      template < class U >
      CORBARefCounted( const CORBARefCounted< U >& )
      {}

      static P Clone( const P& val )
      {
	if ( ! CORBA::is_nil( val ) )
	  val->AddRef();

	return val;
      }

      static bool Release( const P& val )
      {
	if ( ! CORBA::is_nil( val ) )
	  val->Release();

	return false;
      }

      enum { destructiveCopy = false };
      
      static void Swap(CORBARefCounted&)
      {}
    };


    //---------------------------------------------------------------------------
    template< class P >
    struct CORBAComparision
    {
      static P Equal( const P& theLeft, const P& theRight )
      {
        return theLeft->equal( theRight );
      }
      
      static bool Less( const P& theLeft, const P& theRight )
      {
        return theLeft->less( theRight );
      }
    };


   //---------------------------------------------------------------------------
    template
    <
      typename T,
      template <class> class OwnershipPolicy = CORBARefCounted,
      class ConversionPolicy = Loki::DisallowConversion,
      template <class> class CheckingPolicy = Loki::AssertCheck,
      template <class> class StoragePolicy = CORBAStorage,
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
