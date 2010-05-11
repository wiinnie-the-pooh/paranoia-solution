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
#ifndef dev_TTaskMgr_h
#define dev_TTaskMgr_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <list>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct TTaskMgr : public base::TTaskMgr
    {
      static base::TTaskMgrPtr get_instance();
      
      virtual void start();
      virtual void wait();
      virtual void stop();
      virtual void pause();
      virtual void resume();
      virtual bool is_run();
      
      virtual void add_task( base::TTaskPtr theTask );
      
      virtual void connect( const base::TTaskPtr& theSource, 
                            const std::string& theSourceName,
                            const base::TTaskPtr& theDest, 
                            const std::string& theDestName ) throw( std::bad_typeid );
    protected:
      TTaskMgr();
      
    private:
      typedef std::list< base::TTaskPtr > TaskList;
      TaskList m_Tasks;
      
      boost::thread_group m_Threads;
      
      bool m_IsRun;
      boost::mutex m_Pause;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
