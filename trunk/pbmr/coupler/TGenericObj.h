#ifndef GENERICOBJ_HEADER
#define GENERICOBJ_HEADER

#include "loki/Threads.h"

/*
  Defines the intruisive counted base object 
*/

struct TGenericObj;

struct TGenericObj : Loki::ObjectLevelLockable< TGenericObj >
{
  void Register();
  void Destroy();

  unsigned long GetRefCount();

protected:
  TGenericObj();
  virtual ~TGenericObj() {}

private:
  unsigned long m_ref_counter;

  TGenericObj( const TGenericObj& );
  void operator = ( const TGenericObj& );
};


#endif
