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
#include "parallel/dev/TQueue.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    TQueue::TQueue()
    {
      pthread_mutexattr_t attr1;
      pthread_mutexattr_init( &attr1 );
      pthread_mutexattr_settype( &attr1, PTHREAD_MUTEX_NORMAL );
      pthread_mutex_init( &m_Read, &attr1 );
      
      pthread_mutexattr_t attr2;
      pthread_mutexattr_init( &attr2 );
      pthread_mutexattr_settype( &attr2, PTHREAD_MUTEX_NORMAL );
      pthread_mutex_init( &m_List, &attr2 );
      
      pthread_mutex_lock( &m_Read );
    }


    //---------------------------------------------------------------------------
    void TQueue::publish( const base::TDataHolderPtr& theDataHolder )
    {
      pthread_mutex_lock( &m_List );
      
      m_DataHolders.push_back( theDataHolder );
      
      pthread_mutex_unlock( &m_List );
      pthread_mutex_unlock( &m_Read );
    }


    //---------------------------------------------------------------------------
    base::TDataHolderPtr TQueue::retrieve()
    {
      pthread_mutex_lock( &m_Read );
      pthread_mutex_lock( &m_List );
      
      base::TDataHolderPtr aDataHolder;
      if ( !m_DataHolders.empty() )
      {
        aDataHolder = m_DataHolders.front();
        m_DataHolders.pop_front();
      }
      
      // "publish" could be called many times before first "retrive" will be called
      // we should be able read all the available data
      if ( !m_DataHolders.empty() )
        pthread_mutex_unlock( &m_Read );
      
      pthread_mutex_unlock( &m_List );
      
      return aDataHolder;
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
