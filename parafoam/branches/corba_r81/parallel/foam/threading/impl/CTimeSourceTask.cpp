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
#include "parallel/foam/threading/impl/CTimeSourceTask.h"

#include "parallel/foam/threading/impl/SFoamMutex.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    CTimeSourceTask::CTimeSourceTask()
      : m_delta_i( "delta", true, *this )
      , m_next_i( "next", true, *this )

      , m_finished_o( "finished", false, *this )
      , m_time_o( "time", false, *this )
      , m_index_o( "index", false, *this )
      , m_write_o( "write", false, *this )

      , m_end( 0.0 )
      , m_writeInterval( 1 )
    {}


    //-----------------------------------------------------------------------
    void CTimeSourceTask::init()
    {
      MSG( "Start of CTimeSourceTask[ " << this << " ]\n" );
      
      this->m_next_i.init( true );
      this->m_delta_i.init( this->m_delta_i() );
    }
    
    
    //-----------------------------------------------------------------------
    bool CTimeSourceTask::step()
    {
      //SFoamMutex aMutex; // this line leads to dead lock by somereasons

      this->m_next_i.retrieve();
      this->m_delta_i.retrieve();

      if ( this->m_next_i() ) 
      {
        this->increment();
      }

      Foam::dimensionedScalar time_eps( this->deltaT() * 0.01 );
      bool is_time_exit = this->value() - time_eps > this->endTime();
      this->m_finished_o.publish( is_time_exit && this->m_next_i() );

      MSG( "CTimeSourceTask[ " << this << " ]::step"
           << " | value = " << this->value().value()
           << " | is_time_exit = " << is_time_exit
           << " | m_finished_o = " << this->m_finished_o()
           << " | m_next_i = " << this->m_next_i() << "\n" ); 

      this->m_time_o.publish();
      this->m_index_o.publish();

      bool is_write = this->timeIndex() % this->getWriteInterval() == 0;
      this->m_write_o.publish( is_write );

      return ! this->m_finished_o();
    }


    //-----------------------------------------------------------------------
    void CTimeSourceTask::destroy()
    {
      MSG( "End of CTimeSourceTask[ " << this << " ]\n" );
    }
    

    //-----------------------------------------------------------------------
    void CTimeSourceTask::increment()
    {
      this->setTime( this->value() + this->deltaT(), this->timeIndex() + 1 );
    }
    

    //-----------------------------------------------------------------------
    void CTimeSourceTask::setTime( const dimensionedScalar& newTime, label newIndex )
    {
      this->m_time_o() = newTime;
      this->m_index_o() = newIndex;
    }
    
    const dimensionedScalar& CTimeSourceTask::value() const
    {
      return this->m_time_o();
    }
    
    label CTimeSourceTask::timeIndex() const
    {
      return this->m_index_o();
    }
    
    
    //-----------------------------------------------------------------------
    void CTimeSourceTask::setDeltaT( const dimensionedScalar& deltaT )
    {
      this->m_delta_i() = deltaT;
    }
    
    const dimensionedScalar& CTimeSourceTask::deltaT() const
    {
      return this->m_delta_i();
    }
    
    
    //-----------------------------------------------------------------------
    void CTimeSourceTask::setEndTime( const dimensionedScalar& endTime )
    {
      this->m_end = endTime;
    }
    
    const dimensionedScalar& CTimeSourceTask::endTime() const
    {
      return this->m_end;
    }
    

    //-----------------------------------------------------------------------
    void CTimeSourceTask::setWriteInterval( label writeInterval )
    {
      this->m_writeInterval = writeInterval;
    }
    
    label CTimeSourceTask::getWriteInterval() const
    {
      return this->m_writeInterval;
    }
    
    
    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
