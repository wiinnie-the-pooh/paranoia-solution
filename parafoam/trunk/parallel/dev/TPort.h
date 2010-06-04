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
