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
#ifndef Interfaces_h
#define Interfaces_h


//---------------------------------------------------------------------------
#include "parallel/base/TSmartPtr.h"

#include <string>
#include <map>
#include <typeinfo>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace base
  {
    //---------------------------------------------------------------------------
    struct TDataHolder : TGenericObj
    {};
    typedef SmartPtrDef< TDataHolder >::type TDataHolderPtr;


    //---------------------------------------------------------------------------
    struct TQueue : TGenericObj
    {
      virtual void publish( const TDataHolderPtr& theDataHolder ) = 0;
      virtual TDataHolderPtr retrieve() = 0;
    };
    typedef SmartPtrDef< TQueue >::type TQueuePtr;


    //---------------------------------------------------------------------------
    struct TPort;
    typedef SmartPtrDef< TPort >::type TPortPtr;
    
    struct TPort : TGenericObj
    {
      virtual std::string name() = 0;
      virtual void is_compatible( const TPortPtr& thePort ) throw( std::bad_typeid ) = 0;
    };


    //---------------------------------------------------------------------------
    struct TTask;
    typedef SmartPtrDef< TTask >::type TTaskPtr;
    
    struct TTaskMgr;
    typedef SmartPtrDef< TTaskMgr >::type TTaskMgrPtr;
    
    typedef std::map< std::string, TPortPtr > TName2Port;
    
    struct TTask : TGenericObj
    {
      virtual TName2Port get_input_ports() = 0;
      virtual TName2Port get_output_ports() = 0;
      
      virtual TPortPtr get_input_port( const std::string& theName ) = 0;
      virtual TPortPtr get_output_port( const std::string& theName ) = 0;
      
      virtual void connect_input( const TPortPtr& thePort, 
                                  const TQueuePtr& theQueue, 
                                  const TPortPtr& theOppositePort ) throw( std::bad_typeid ) = 0;
      
      virtual void connect_output( const TPortPtr& thePort, 
                                   const TQueuePtr& theQueue, 
                                   const TPortPtr& theOppositePort ) throw( std::bad_typeid ) = 0;
      
      virtual void invoke( TTaskMgrPtr theMgr ) = 0;
      
    protected:
      virtual void init() = 0;
      virtual void destroy() = 0;
      virtual bool step() = 0;
    };


    //---------------------------------------------------------------------------
    struct TTaskMgr : TGenericObj
    {
      virtual void start() = 0;
      virtual void wait() = 0;
      virtual void stop() = 0;
      virtual void pause() = 0;
      virtual void resume() = 0;
      virtual bool is_run() = 0;
      
      virtual void add_task( TTaskPtr theTask ) = 0;
      
      virtual void connect( const TTaskPtr& theSource, 
                            const std::string& theSourceName,
                            const TTaskPtr& theDest, 
                            const std::string& theDestName ) throw( std::bad_typeid ) = 0;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
