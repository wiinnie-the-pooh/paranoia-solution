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
#ifndef CTimeSourceTask_h
#define CTimeSourceTask_h


//---------------------------------------------------------------------------
#include "parallel/threading/dev/TTask.h"

#include "parallel/threading/dev/CPortHelper.h"

#include "parallel/threading/dev/CBoolPort.h"

#include "parallel/threading/dev/CIntPort.h"

#include "parallel/foam/threading/impl/CDimensionedScalar.h"
#include "parallel/foam/threading/impl/CSerializedDimensionedScalar.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    using namespace Foam;


    //-----------------------------------------------------------------------
    struct CTimeSourceTask : dev::TTask
    {
      typedef base::SmartPtrDef< CTimeSourceTask >::type TPtr;

      CTimeSourceTask();
      
      void setTime( const dimensionedScalar& newTime, label newIndex );
      const dimensionedScalar& value() const;
      virtual label timeIndex() const;
      
      void setDeltaT( const dimensionedScalar& deltaT );
      const dimensionedScalar& deltaT() const;
      
      void setEndTime( const dimensionedScalar& endTime );
      const dimensionedScalar& endTime() const;
      
      void setWriteInterval( label writeInterval );
      label getWriteInterval() const;
      
    protected:
      dev::CPortHelper< CSerializedDimensionedScalar > m_delta_i;
      dev::CPortHelper< dev::CBoolPort > m_next_i;

      dev::CPortHelper< dev::CBoolPort > m_finished_o;
      dev::CPortHelper< CSerializedDimensionedScalar > m_time_o;
      dev::CPortHelper< dev::CIntPort > m_index_o;
      dev::CPortHelper< dev::CBoolPort > m_write_o;

      Foam::dimensionedScalar m_end;
      Foam::label m_writeInterval;
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();

      void increment();
    };
    
    
    //---------------------------------------------------------------------------
  }
}

//---------------------------------------------------------------------------
#endif
