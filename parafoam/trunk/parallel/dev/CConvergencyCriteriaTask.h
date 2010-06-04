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
#ifndef dev_CConvergencyCriteriaTask_h
#define dev_CConvergencyCriteriaTask_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/dev/CBoolPort.h"

#include "parallel/dev/CDoublePort.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct CConvergencyCriteriaTask : TTask
    {
      typedef base::SmartPtrDef< CConvergencyCriteriaTask >::type TPtr;
 
      CConvergencyCriteriaTask();

      ~CConvergencyCriteriaTask();

      void setMaxResidual( double the_residual );
      double getMaxResidual() const;
      
      void setMaxIterationNumber( int the_number );
      int getMaxIterationNumber() const;
      
    protected:
      CPortHelper< CDoublePort > m_residual_i;
      CPortHelper< CBoolPort > m_stop_i;

      CPortHelper< CBoolPort > m_finished_o;
      CPortHelper< CBoolPort > m_result_o;

    protected:
      double maxResidual;
      int maxIterationNumber;
      int iterationCounter;

    protected:
      virtual void init();
      virtual bool step();
      virtual void destroy();
    };


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
