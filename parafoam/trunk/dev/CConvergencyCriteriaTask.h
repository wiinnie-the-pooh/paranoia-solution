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
