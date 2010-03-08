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
#include "parallel/foam/CSolverTaskBase.h"


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
      , m_next_i( "next", true, *this )
      , m_stop_i( "stop", true, *this )
      , m_write_i( "write", true, *this )
      , m_continue_i( "continue", true, *this )
      , m_residual_o( "residual", false, *this )
      , m_finished_o( "finished", false, *this )
    {}

    //-----------------------------------------------------------------------
    CSolverTaskBase::~CSolverTaskBase()
    {}

 
    //-----------------------------------------------------------------------
    void CSolverTaskBase::init()
    {
      std::cout << "\nStart of CSolverTaskBase[ " << this << " ]\n";

      this->m_continue_i.init( true );
    }
    
    
    //-----------------------------------------------------------------------
    bool CSolverTaskBase::pre_step()
    {
      this->m_continue_i.retrieve();

      this->m_time_i.retrieve();
      this->m_index_i.retrieve();

      this->m_next_i.retrieve();
      this->m_stop_i.retrieve();
      this->m_write_i.retrieve();

      if ( this->m_next_i() )
        this->increment();

      this->m_finished_o.publish( this->m_stop_i() );

      return ! this->finished();
    }


    //-----------------------------------------------------------------------
    bool CSolverTaskBase::post_step()
    {
      if ( this->m_write_i() )
        this->runTime.writeNow();

      return ! this->finished();
    }


    //-----------------------------------------------------------------------
    void CSolverTaskBase::increment()
    {
      Info << endl << "CSolverTaskBase::increment[ " << this->m_time_i() << " ] = " << this->m_index_i() << endl;
      this->runTime.setTime( this->m_time_i(), this->m_index_i() );
    }
    

    //-----------------------------------------------------------------------
    bool CSolverTaskBase::finished()
    {
      return this->m_finished_o();
    }
    

    //-----------------------------------------------------------------------
    void CSolverTaskBase::destroy()
    {
      std::cout << "\nEnd of CSolverTaskBase[ " << this << " ]\n";
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
