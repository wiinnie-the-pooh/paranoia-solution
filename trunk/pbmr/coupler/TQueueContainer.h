
#ifndef QUEUE_CONTAINER_HEADER
#define QUEUE_CONTAINER_HEADER

#include <Interfaces.h>

namespace Impl
{
  class TQueueContainer : public Base::TLink
  {
  public:
    TQueueContainer();

  protected:
    Base::TQueuePtr get_queue( int theSubDomain );

  private:
    typedef std::map<int, Base::TQueuePtr> QueuesMap;
    QueuesMap m_Queues;
    pthread_mutex_t m_Queue;
  };
}

#endif
