
#include <TQueueContainer.h>
#include <TQueue.h>

namespace Impl {

TQueueContainer::TQueueContainer()
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init( &attr );
  pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
  pthread_mutex_init( &m_Queue, &attr );
}

Base::TQueuePtr TQueueContainer::get_queue( int theSubDomain )
{
  pthread_mutex_lock( &m_Queue );
  Base::TQueuePtr aQueue;
  QueuesMap::const_iterator it = m_Queues.find( theSubDomain );
  if( it==m_Queues.end() )
  {
    aQueue = Base::TQueuePtr( new TQueue() );
    m_Queues[theSubDomain] = aQueue;
  }
  else
    aQueue = (*it).second;
  pthread_mutex_unlock( &m_Queue );
  return aQueue;
}

}
