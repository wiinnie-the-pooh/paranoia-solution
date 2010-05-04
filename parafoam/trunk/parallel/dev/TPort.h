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
#ifndef dev_TPort_h
#define dev_TPort_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"


//---------------------------------------------------------------------------
#define PARALLEL_DERIVED_PORT_DEF( DerivedPort ) \
  DerivedPort( const std::string& theName, parallel::dev::TTask& theTask ) \
  : parallel::dev::TPort( theName, theTask )                         \
  {} \
  \
  virtual void is_compatible( const base::TPortPtr& thePort ) throw( std::bad_typeid ) \
  { \
    if ( dynamic_cast< const DerivedPort* >( thePort.GetPointer( thePort ) ) == NULL ) \
      throw std::bad_typeid(); \
  }


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct TTask;
    typedef base::SmartPtrDef< TTask >::type TTaskPtr;


    //---------------------------------------------------------------------------
    struct TPort : base::TPort
    {
      TPort( const std::string& theName, TTask& theTask );

      virtual std::string name();
      
      base::TTaskPtr task();
      
      TTaskPtr dev_task();
      
    protected:
      std::string m_name;
      TTask& m_task;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif