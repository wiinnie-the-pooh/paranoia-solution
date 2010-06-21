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
#ifndef dev_TTask_h
#define dev_TTask_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"

#include <set>

#include <sstream>


//---------------------------------------------------------------------------
#define MSG( the_message ) { std::ostringstream a_stream; a_stream << the_message; parallel::dev::TTask::print( *this, a_stream.str() ); }

//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct CPortHelperEngine;


    //---------------------------------------------------------------------------
    struct TTask : public base::TTask
    {
      virtual base::TName2Port get_input_ports();
      virtual base::TName2Port get_output_ports();
      
      virtual base::TPortPtr get_input_port( const std::string& theName );
      virtual base::TPortPtr get_output_port( const std::string& theName );
      
      virtual void connect_input( const base::TPortPtr& thePort, 
                                  const base::TQueuePtr& theQueue, 
                                  const base::TPortPtr& theOppositePort ) throw( std::bad_typeid );

      virtual void connect_output( const base::TPortPtr& thePort, 
                                   const base::TQueuePtr& theQueue, 
                                   const base::TPortPtr& theOppositePort ) throw( std::bad_typeid );

      virtual void invoke( base::TTaskMgrPtr theMgr );
      
    protected:
      bool define_input_port( const base::TPortPtr& thePort );
      bool define_output_port( const base::TPortPtr& thePort );
      
      base::TDataHolderPtr wait_for( const std::string& theName );

      template< class DataHolderType >
      typename base::SmartPtrDef< DataHolderType >::type 
      retrieve( const std::string& theName )
      {
        typename base::TDataHolderPtr aDataHolder = this->wait_for( theName );
        if ( !aDataHolder )
          return NULL;
        
        typename base::TDataHolder* aHolder = aDataHolder.GetPointer( aDataHolder );
        aHolder->AddRef();
        return dynamic_cast< DataHolderType* >( aHolder );
      }

      void init_port( const std::string& theName, const base::TDataHolderPtr& theDataHolder );
      
      void publish( const std::string& theName, const base::TDataHolderPtr& theDataHolder );

      friend struct CPortHelperEngine;

    protected:
      TTask();

      virtual void init() 
      {}
      
      virtual void destroy()
      {}

      static void print( TTask& theTask, const std::string& theMessage );

      int step_counter() const;

    private:
      typedef std::pair< base::TQueuePtr, base::TDataHolderPtr > TDataFactrory;
      typedef std::map< base::TPortPtr, TDataFactrory > TInputPorts;
      TInputPorts m_InputPorts;
      
      typedef std::set< base::TQueuePtr > TQueueSet;
      typedef std::map< base::TPortPtr, TQueueSet > TOutputPorts;
      TOutputPorts m_OutputPorts;

      int m_StepCounter;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
