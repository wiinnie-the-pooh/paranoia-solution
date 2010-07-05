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
#include "parallel/foam/threading/impl/CSolverTaskBase.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    CSolverTaskBase::CSolverTaskBase( Time& runTime )
      : runTime( runTime )
      , m_time_i( "time", true, *this )
      , m_index_i( "index", true, *this )
      , m_write_i( "write", true, *this )
      , m_stop_i( "stop", true, *this )

      , m_finished_o( "finished", false, *this )
      , m_residual_o( "residual", false, *this )
    {}

    //-----------------------------------------------------------------------
    CSolverTaskBase::~CSolverTaskBase()
    {}

 
    //-----------------------------------------------------------------------
    void CSolverTaskBase::init()
    {
      MSG( "Start of CSolverTaskBase[ " << this << " ]\n" );
    }
    
    
    //-----------------------------------------------------------------------
    bool CSolverTaskBase::pre_step()
    {
      this->m_finished_o.publish( this->m_stop_i.retrieve() );

      this->m_time_i.retrieve();
      this->m_index_i.retrieve();

      this->runTime.setTime( this->m_time_i(), this->m_index_i() );

      return ! this->finished();
    }


    //-----------------------------------------------------------------------
    bool CSolverTaskBase::post_step()
    {
      if ( this->m_write_i.retrieve() )
        this->runTime.writeNow();

      return ! this->finished();
    }


    //-----------------------------------------------------------------------
    bool CSolverTaskBase::finished()
    {
      return this->m_finished_o();
    }
    

    //-----------------------------------------------------------------------
    void CSolverTaskBase::destroy()
    {
      MSG( "End of CSolverTaskBase[ " << this << " ]\n" );
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
