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
#ifndef dev_CORPredicateTask_h
#define dev_CORPredicateTask_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/dev/CBoolPort.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    struct CORPredicateTask : TTask
    {
      typedef base::SmartPtrDef< CORPredicateTask >::type TPtr;
 
      CORPredicateTask();

      ~CORPredicateTask();

    protected:
      CPortHelper< CBoolPort > m_left_i;
      CPortHelper< CBoolPort > m_stop_left_i;

      CPortHelper< CBoolPort > m_right_i;
      CPortHelper< CBoolPort > m_stop_right_i;

      CPortHelper< CBoolPort > m_finished_o;
      CPortHelper< CBoolPort > m_result_o;

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
