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
#ifndef dev_CPortHelperEngine_h
#define dev_CPortHelperEngine_h


//---------------------------------------------------------------------------
#include "parallel/threading/base/Interfaces.h"

#include "parallel/threading/dev/TTask.h"

#include "parallel/threading/dev/TPort.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct CPortHelperEngine
    {
    protected:
      typedef base::SmartPtrDef< TPort >::type TPortPtr;

      //---------------------------------------------------------------------------
      CPortHelperEngine( const TPortPtr thePort, bool theIsInput )
      : port( thePort ) 
      {
        if ( theIsInput )
          this->task()->define_input_port( thePort );
        else
          this->task()->define_output_port( thePort );
      }

      //---------------------------------------------------------------------------
      std::string name()
      {
        return this->port->name();
      }
      
      //---------------------------------------------------------------------------
      TTaskPtr task()
      {
        return this->port->dev_task();
      }

      //---------------------------------------------------------------------------
      void __init__( const TTaskPtr& theTask,
                          const std::string& theName, 
                          const base::TDataHolderPtr& theDataHolder )
      {
        theTask->init_port( theName, theDataHolder );
      }

      //---------------------------------------------------------------------------
      template< class DataHolderType >
      typename base::SmartPtrDef< DataHolderType >::type 
      __retrieve__( const TTaskPtr& theTask,
                    const std::string& theName )
      {
        return theTask->template retrieve< DataHolderType >( theName );
      }

      //---------------------------------------------------------------------------
      void __publish__( const TTaskPtr& theTask,
                        const std::string& theName, 
                        const base::TDataHolderPtr& theDataHolder )
      {
        theTask->publish( theName, theDataHolder );
      }

    protected:
      //---------------------------------------------------------------------------
      TPortPtr port;
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
