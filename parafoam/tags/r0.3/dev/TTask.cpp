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
#include "parallel/dev/TTask.h"
#include "parallel/dev/TPort.h"

#include <iostream>


//---------------------------------------------------------------------------
namespace
{
  using namespace parallel;
  base::TPortPtr
  find_port( const base::TName2Port& theName2Port, 
             const std::string& theName )
  {
    base::TName2Port::const_iterator anIter = theName2Port.find( theName );
    if ( anIter != theName2Port.end() )
      return anIter->second;

    return NULL;
  }

  
  //---------------------------------------------------------------------------
  static pthread_mutex_t PRINT_MUTEX;

  int init()
  {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init( &attr );
    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
    pthread_mutex_init( &PRINT_MUTEX, &attr );

    return 1;
  }

  static int INIT = init();
}


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    bool TTask::define_input_port( const base::TPortPtr& thePort )
    {
      if ( m_InputPorts.find( thePort ) != m_InputPorts.end() )
        return false;
      
      m_InputPorts[ thePort ];
      
      return true;
    }
    
    
    //---------------------------------------------------------------------------
    bool TTask::define_output_port( const base::TPortPtr& thePort )
    {
      if ( m_OutputPorts.find( thePort ) != m_OutputPorts.end() )
        return false;
      
      m_OutputPorts[ thePort ];
      
      return true;
    }
    
    
    //---------------------------------------------------------------------------
    base::TName2Port TTask::get_input_ports()
    {
      base::TName2Port aResult;
      
      TInputPorts::const_iterator anIter = m_InputPorts.begin();
      TInputPorts::const_iterator anEnd = m_InputPorts.end();
      for ( ; anIter != anEnd; anIter++ ) {
        base::TPortPtr aPort = anIter->first;
        aResult[ aPort->name() ] = aPort;
      }
      
      return aResult;
    }
    
    
    //---------------------------------------------------------------------------
    base::TName2Port TTask::get_output_ports()
    {
      base::TName2Port aResult;
      
      TOutputPorts::const_iterator anIter = m_OutputPorts.begin();
      TOutputPorts::const_iterator anEnd = m_OutputPorts.end();
      for ( ; anIter != anEnd; anIter++ ) {
        base::TPortPtr aPort = anIter->first;
        aResult[ aPort->name() ] = aPort;
      }
      
      return aResult;
    }
    
    
    //---------------------------------------------------------------------------
    base::TPortPtr TTask::get_input_port( const std::string& theName )
    {
      return find_port( this->get_input_ports(), theName );
    }
    
    
    //---------------------------------------------------------------------------
    base::TPortPtr TTask::get_output_port( const std::string& theName )
    {
      return find_port( this->get_output_ports(), theName );
    }
    
    
    //---------------------------------------------------------------------------
    void TTask::connect_input( const base::TPortPtr& thePort, 
                               const base::TQueuePtr& theQueue, 
                               const base::TPortPtr& theOppositePort ) throw( std::bad_typeid )
    {
      thePort->is_compatible( theOppositePort );
      
      TDataFactrory& aDataFactrory = m_InputPorts[ thePort ];
      base::TQueuePtr& aQueue = aDataFactrory.first;
      
      aQueue = theQueue;
    }
    
    
    //---------------------------------------------------------------------------
    void TTask::connect_output( const base::TPortPtr& thePort, 
                                const base::TQueuePtr& theQueue, 
                                const base::TPortPtr& theOppositePort ) throw( std::bad_typeid )
    {
      thePort->is_compatible( theOppositePort );
      
      m_OutputPorts[ thePort ].insert( theQueue );
    }
    

    //---------------------------------------------------------------------------
    void TTask::invoke( base::TTaskMgrPtr theMgr )
    {
      this->init();
      
      while( theMgr->is_run() && this->step() )
        this->m_StepCounter++;
      
      this->destroy();
    }
    
    
    //---------------------------------------------------------------------------
    base::TDataHolderPtr TTask::wait_for( const std::string& theName )
    {
      base::TName2Port aName2Port = this->get_input_ports();
      
      const base::TPortPtr& aPort = aName2Port[ theName ];
      TDataFactrory& aDataFactrory = m_InputPorts[ aPort ];
      
      if ( const base::TQueuePtr& aQueue = aDataFactrory.first )
        return aQueue->retrieve();
      else
      {
        const base::TDataHolderPtr& aDataHolderPtr = aDataFactrory.second;
        return aDataHolderPtr;
      }
    }
    
    
    //---------------------------------------------------------------------------
    void TTask::init_port( const std::string& theName, const base::TDataHolderPtr& theDataHolder )
    {
      base::TName2Port aName2Port = this->get_input_ports();
      
      const base::TPortPtr& aPort = aName2Port[ theName ];
      TDataFactrory& aDataFactrory = m_InputPorts[ aPort ];
      
      if ( const base::TQueuePtr& aQueue = aDataFactrory.first )
        aQueue->publish( theDataHolder );
      else
      {
        base::TDataHolderPtr& aDataHolderPtr = aDataFactrory.second;
        aDataHolderPtr = theDataHolder;
      }
    }
    
    
    //---------------------------------------------------------------------------
    void TTask::publish( const std::string& theName, const base::TDataHolderPtr& theDataHolder )
    {
      base::TName2Port aName2Port = this->get_output_ports();
      
      const base::TPortPtr& aPort = aName2Port[ theName ];
      const TQueueSet& aQueueSet = m_OutputPorts[ aPort ];
      
      TQueueSet::const_iterator aQueueIter = aQueueSet.begin();
      TQueueSet::const_iterator aQueueEnd = aQueueSet.end();
      for( ; aQueueIter != aQueueEnd; aQueueIter++ ) {
        const base::TQueuePtr& aQueue = *aQueueIter;
        //std::cout << "TTask::publish - " << aQueue.GetPointer( aQueue ) << "\n";
        aQueue->publish( theDataHolder );
      }
    }
      
      
    //---------------------------------------------------------------------------
    TTask::TTask()
      : m_StepCounter( 0 )
    {}
    

    //---------------------------------------------------------------------------
    void TTask::print( TTask& theTask, const std::string& theMessage )
    {
      pthread_mutex_lock( &PRINT_MUTEX );

      std::cout << "\n< " << theTask.step_counter() << " > - " << theMessage;

      pthread_mutex_unlock( &PRINT_MUTEX );
    }


    //---------------------------------------------------------------------------
    int TTask::step_counter() const
    {
      return this->m_StepCounter;
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
