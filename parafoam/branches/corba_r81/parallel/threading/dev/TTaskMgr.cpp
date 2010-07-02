// VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
// Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/threading/dev/TTaskMgr.h"
#include "parallel/threading/dev/TQueue.h"

#include "parallel/threading/dev/boost_threading.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <algorithm>
#include <iostream>


//---------------------------------------------------------------------------
namespace
{
    //---------------------------------------------------------------------------
    void invoke( const parallel::base::TTaskPtr& theTask )
    {
      theTask->invoke( parallel::dev::TTaskMgr::get_instance() );
    }


    //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    TTaskMgr::TTaskMgr()
      : m_IsRun( false )
    {}


    //---------------------------------------------------------------------------
    base::TTaskMgrPtr TTaskMgr::get_instance()
    {
      static base::TTaskMgrPtr anInst = base::TTaskMgrPtr( new TTaskMgr() );

      return anInst;
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
        const base::TTaskPtr& aTask = *anIter;
        m_Threads.create_thread( boost::bind( boost::bind< void >( invoke, _1 ), aTask ) );
      }
    }

    
    //---------------------------------------------------------------------------
    void TTaskMgr::wait()
    {
      m_Threads.join_all();
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::stop()
    {
      m_IsRun = false;
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::pause()
    {
      parallel::threading::lock( m_Pause );
    }


    //---------------------------------------------------------------------------
    void TTaskMgr::resume()
    {
      parallel::threading::unlock( m_Pause );
    }


    //---------------------------------------------------------------------------
    bool TTaskMgr::is_run()
    {
      parallel::threading::lock( m_Pause );
      parallel::threading::unlock( m_Pause );

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
