
#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include <Interfaces.h>
#include <list>

namespace Impl
{
  class TQueue : public Base::TQueue
  {
  public:
    TQueue();

    virtual void write( Base::TFieldPtr theField );
    virtual Base::TFieldPtr read();

  private:
    std::list<Base::TFieldPtr> m_Fields;
    pthread_mutex_t m_List, m_Read;
  };
}

#endif
