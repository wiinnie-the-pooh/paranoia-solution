
#include <TQueue.h>
#include <iostream>

namespace Impl {

TQueue::TQueue()
{
  pthread_mutexattr_t attr1, attr2;
  pthread_mutexattr_init( &attr1 );
  pthread_mutexattr_init( &attr2 );
  pthread_mutexattr_settype( &attr1, PTHREAD_MUTEX_NORMAL );
  pthread_mutexattr_settype( &attr2, PTHREAD_MUTEX_NORMAL );
  pthread_mutex_init( &m_Read, &attr1 );
  pthread_mutex_init( &m_List, &attr2 );

  pthread_mutex_lock( &m_Read );
}

void TQueue::write( Base::TFieldPtr theField )
{
  //printf( "TQueue %i ::write\n", (int)this );// << (char*)theField->get_raw_data() << std::endl;
  pthread_mutex_lock( &m_List );
  m_Fields.push_back( theField );
  pthread_mutex_unlock( &m_List );
  pthread_mutex_unlock( &m_Read );
}

Base::TFieldPtr TQueue::read()
{
  //printf( "TQueue %i ::read\n", (int)this );
  pthread_mutex_lock( &m_Read );
  pthread_mutex_lock( &m_List );
  Base::TFieldPtr aField;
  //printf( "TQueue::read, get data\n" );
  if( !m_Fields.empty() )
  {
    aField = *m_Fields.begin();
    m_Fields.remove( aField );
    //std::cout << "TQueue::read: data extracted: " << (char*)aField->get_raw_data() << std::endl;
  }
  //else
  //  printf( "THE QUEUE %i IS EMPTY\n", (int)this );

  if( !m_Fields.empty() )
    pthread_mutex_unlock( &m_Read );

  pthread_mutex_unlock( &m_List );
  return aField;
}

}
