
#ifndef FIELD_HEADER
#define FIELD_HEADER

#include "Interfaces.h"

namespace Impl
{
  class TField : public Base::TField
  {
  public:
    TField( const std::string& theName, Base::TData theData, int theSize );
    TField( const std::string& theName, int theSubDomain, Base::TData theData, int theSize );
    virtual ~TField();

    virtual std::string get_name();
    virtual int get_subdomain();
    virtual Base::TData get_raw_data();
    virtual int get_size();

  private:
    void init( Base::TData theData, int theSize );

  private:
    std::string m_Name;
    int m_SubDomain, m_Size;
    Base::TData m_Data;
  };
}

#endif
