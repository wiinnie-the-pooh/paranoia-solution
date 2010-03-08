#ifndef SMARTPTR_HEADER
#define SMARTPTR_HEADER

#include "TGenericObj.h"

#include "loki/SmartPtr.h"

/*
  Defines appropriate SmartPtr for the given package 
*/


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
#endif
