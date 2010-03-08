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
#include "parallel/dev/CORPredicateTask.h"

#include <iostream>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //-----------------------------------------------------------------------
    CORPredicateTask::CORPredicateTask()
      : m_left_i( "left", true, *this )
      , m_stop_left_i( "stop_left", true, *this )

      , m_right_i( "right", true, *this )
      , m_stop_right_i( "stop_right", true, *this )

      , m_finished_o( "finished", false, *this )
      , m_result_o( "result", false, *this )
    {}


    //-----------------------------------------------------------------------
    CORPredicateTask::~CORPredicateTask()
    {}
 

    //-----------------------------------------------------------------------
    void CORPredicateTask::init()
    {
      std::cout << "\nStart of CORPredicateTask[ " << this << " ]\n";
    }
    
    
    //-----------------------------------------------------------------------
    bool CORPredicateTask::step()
    {
      std::cout << "\nCORPredicateTask[ " << this << " ]::step\n";

      this->m_finished_o.publish( this->m_stop_left_i.retrieve() || this->m_stop_right_i.retrieve() );

      std::cout << "\nCORPredicateTask[ " << this << " ]"
		<< " | m_finished_o = " << this->m_finished_o()
		<< "\n"; 

      this->m_result_o.publish( this->m_left_i.retrieve() && this->m_right_i.retrieve() );
      
      return ! this->m_finished_o();
    }


    //-----------------------------------------------------------------------
    void CORPredicateTask::destroy()
    {
      std::cout << "\nEnd of CORPredicateTask[ " << this << " ]\n";
    }
    

    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
