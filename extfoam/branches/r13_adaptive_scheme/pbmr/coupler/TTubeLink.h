
#ifndef TUBE_LINK_HEADER
#define TUBE_LINK_HEADER

#include <Interfaces.h>

namespace Impl
{
  class TTubeLink : public Base::TLink
  {
  public:
    TTubeLink();

    virtual void write( Base::TFieldPtr theField );
    virtual Base::TFieldPtr read();
    virtual Base::TFieldPtr read_domain( int theSubDomain );

  private:
    Base::TQueuePtr m_Queue;
  };
}

#endif
