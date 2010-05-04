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
#ifndef dev_CPortHelperEngine_h
#define dev_CPortHelperEngine_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"

#include "parallel/dev/TTask.h"

#include "parallel/dev/TPort.h"


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
