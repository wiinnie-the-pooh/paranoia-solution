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
#include "parallel/foam/CTimeSourceTask.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    CTimeSourceTask::CTimeSourceTask()
      : m_next_i( "next", true, *this )
      , m_delta_i( "delta", true, *this )

      , m_time_o( "time", false, *this )
      , m_index_o( "index", false, *this )
      , m_next_o( "next", false, *this )
      , m_stop_o( "stop", false, *this )
      , m_write_o( "write", false, *this )

      , m_end( 0.0 )
      , m_writeInterval( 1 )
    {}


    //-----------------------------------------------------------------------
    void CTimeSourceTask::init()
    {
      std::cout << "\nStart of CTimeSourceTask[ " << this << " ]\n";
      
      this->m_next_i.init( true );
      this->m_delta_i.init( this->m_delta_i() );
    }
    
    
    //-----------------------------------------------------------------------
    bool CTimeSourceTask::step()
    {
      this->m_next_i.retrieve();
      this->m_delta_i.retrieve();

      this->m_next_o.publish( this->m_next_i() );

      if ( this->m_next_i() ) 
      {
        this->increment();
      }

      Foam::dimensionedScalar time_eps( this->deltaT() * 0.01 );
      bool is_time_exit = this->value() - time_eps > this->endTime();
      this->m_stop_o.publish( is_time_exit && this->m_next_i() );

      std::cout << "\nCTimeSourceTask[ " << this << " ]::step"
                << " | value = " << this->value().value()
                << " | is_time_exit = " << is_time_exit
                << " | m_stop_o = " << this->m_stop_o()
                << " | m_next_i = " << this->m_next_i() << "\n"; 

      this->m_time_o.publish();
      this->m_index_o.publish();

      bool is_write = this->timeIndex() % this->getWriteInterval() == 0;
      this->m_write_o.publish( is_write );

      return ! this->m_stop_o();
    }


    //-----------------------------------------------------------------------
    void CTimeSourceTask::destroy()
    {
      std::cout << "\nEnd of CTimeSourceTask[ " << this << " ]\n";
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
