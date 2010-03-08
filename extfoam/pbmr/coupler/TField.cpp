
#include "TField.h"
#include <iostream>

namespace Impl {

TField::TField( const std::string& theName, Base::TData theData, int theSize )
: m_Name( theName ), m_SubDomain( -1 )
{
  init( theData, theSize );
}

TField::TField( const std::string& theName, int theSubDomain, Base::TData theData, int theSize )
: m_Name( theName ), m_SubDomain( theSubDomain )
{
  init( theData, theSize );
}

TField::~TField()
{
  free( m_Data );
}

void TField::init( Base::TData theData, int theSize )
{
  m_Data = malloc( theSize );
  m_Size = theSize;
  memcpy( m_Data, theData, theSize );
}

std::string TField::get_name()
{
  return m_Name;
}

int TField::get_subdomain()
{
  return m_SubDomain;
}

Base::TData TField::get_raw_data()
{
  return m_Data;
}

int TField::get_size()
{
  return m_Size;
}

}
