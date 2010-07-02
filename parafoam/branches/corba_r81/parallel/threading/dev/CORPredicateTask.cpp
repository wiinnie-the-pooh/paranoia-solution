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
#include "parallel/threading/dev/CORPredicateTask.h"

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
      MSG( "Start of CORPredicateTask[ " << this << " ]\n" );
    }
    
    
    //-----------------------------------------------------------------------
    bool CORPredicateTask::step()
    {
      MSG( "CORPredicateTask[ " << this << " ]::step\n" );

      this->m_finished_o.publish( this->m_stop_left_i.retrieve() || this->m_stop_right_i.retrieve() );

      this->m_left_i.retrieve();
      this->m_right_i.retrieve();

      this->m_result_o.publish( this->m_left_i() && this->m_right_i() );
      
      MSG( "CORPredicateTask[ " << this << " ]"
           << " | m_result_o = " << this->m_result_o()
           << " | m_left_i = " << this->m_left_i()
           << " | m_right_i = " << this->m_right_i()
           << "\n" ); 

      return ! this->m_finished_o();
    }


    //-----------------------------------------------------------------------
    void CORPredicateTask::destroy()
    {
      MSG( "End of CORPredicateTask[ " << this << " ]\n" );
    }
    

    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
