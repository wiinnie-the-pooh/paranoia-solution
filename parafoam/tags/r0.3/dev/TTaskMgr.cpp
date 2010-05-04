// Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License. 
// 
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details. 
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/dev/TTaskMgr.h"
#include "parallel/dev/TQueue.h"

#include <iostream>
#include <algorithm>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    TTaskMgr::TTaskMgr()
      : m_IsRun( false )
    {
      pthread_mutexattr_t attr;
      pthread_mutexattr_init( &attr );
      pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
      pthread_mutex_init( &m_Pause, &attr );
    }


    //---------------------------------------------------------------------------
    base::TTaskMgrPtr TTaskMgr::get_instance()
    {
      static base::TTaskMgrPtr anInst = base::TTaskMgrPtr( new TTaskMgr() );

      return anInst;
    }
    

    //---------------------------------------------------------------------------
    void* thrd_start( void* ptr )
    {
      base::TTaskPtr aTask = *((base::TTaskPtr*)ptr);

      aTask->invoke( TTaskMgr::get_instance() );

      return 0;
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::start()
    {
      if ( m_Tasks.size() == 0 )
        return;
      
      m_IsRun = true;

      TaskList::const_iterator anIter = m_Tasks.begin();
      TaskList::const_iterator anEnd = m_Tasks.end();
      for( ; anIter != anEnd; anIter++ )
      {
        pthread_t thrd;
        const base::TTaskPtr& aTask = *anIter;
        pthread_create( &thrd, NULL, thrd_start, (void*)&aTask );
        
        m_Threads.push_back( thrd );
      }
    }

    
    //---------------------------------------------------------------------------
    void TTaskMgr::wait()
    {
      ThreadList::const_iterator anIter = m_Threads.begin();
      ThreadList::const_iterator anEnd = m_Threads.end();
      for ( ; anIter != anEnd; anIter++ )
      {
        pthread_join( *anIter, NULL );
      }
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::stop()
    {
      m_IsRun = false;
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::pause()
    {
      pthread_mutex_lock( &m_Pause );
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::resume()
    {
      pthread_mutex_unlock( &m_Pause );
    }


    //---------------------------------------------------------------------------
    bool TTaskMgr::is_run()
    {
      pthread_mutex_lock( &m_Pause );
      pthread_mutex_unlock( &m_Pause );
      return m_IsRun;
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::add_task( base::TTaskPtr theTask )
    {
      if( find( m_Tasks.begin(), m_Tasks.end(), theTask ) == m_Tasks.end() )
        m_Tasks.push_back( theTask );
    }
    

    //---------------------------------------------------------------------------
    void TTaskMgr::connect( const base::TTaskPtr& theSource, 
                            const std::string& theSourceName,
                            const base::TTaskPtr& theDest, 
                            const std::string& theDestName ) throw( std::bad_typeid )
    {
      if( !theSource || !theDest )
        throw std::bad_typeid();
      
      base::TQueuePtr aQueue( new TQueue() );
      
      base::TPortPtr an_output_port = theSource->get_output_port( theSourceName );
      base::TPortPtr an_input_port = theDest->get_input_port( theDestName );
      
      theSource->connect_output( an_output_port, aQueue, an_input_port );
      theDest->connect_input( an_input_port, aQueue, an_output_port );
    }


    //---------------------------------------------------------------------------
  }
}
    

//---------------------------------------------------------------------------
