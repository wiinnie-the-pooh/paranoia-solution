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
#ifndef CTimeSourceTask_h
#define CTimeSourceTask_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/dev/CBoolPort.h"

#include "parallel/dev/CIntPort.h"

#include "parallel/foam/CDimensionedScalar.h"
#include "parallel/foam/CSerializedDimensionedScalar.h"


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