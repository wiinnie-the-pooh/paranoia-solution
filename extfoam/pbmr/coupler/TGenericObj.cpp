#include "TGenericObj.h"


//---------------------------------------------------------------------------
TGenericObj::TGenericObj()
{
  Lock a_lock( *this );
  m_ref_counter = 1;
}


void TGenericObj::Register()
{
  Lock a_lock( *this );
  ++m_ref_counter;
}

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

unsigned long TGenericObj::GetRefCount()
{
  return m_ref_counter;
}

