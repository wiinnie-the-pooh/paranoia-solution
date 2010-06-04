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
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"
#include "parallel/dev/TPort.h"

#include <boost/thread/mutex.hpp>

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
  static boost::mutex PRINT_MUTEX;
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
      boost::mutex::scoped_lock a_lock( PRINT_MUTEX );

      std::cout << "\n< " << theTask.step_counter() << " > - " << theMessage;
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
