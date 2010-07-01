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
#include "parallel/corba/server/foam/Foam_TimeSourceTask_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  namespace foam
  {
    //---------------------------------------------------------------------------
    TimeSourceTask_i::TimeSourceTask_i( const CORBA::ORB_var& theORB, 
                                        const PortableServer::POA_var& thePOA )
      : TransientObject_i( theORB, thePOA )
      , TaskBase_i( theORB, thePOA )

      , m_delta_i( "delta", eInputPort, this )
      , m_next_i( "next", eInputPort, this )

      , m_finished_o( "finished", eOutputPort, this )
      , m_time_o( "time", eOutputPort, this )
      , m_index_o( "index", eOutputPort, this )
      , m_write_o( "write", eOutputPort, this )

      , m_end( 0.0 )
      , m_writeInterval( 1 )
    {
      cout << "TimeSourceTask_i::TimeSourceTask_i[ " << this << " ]" << endl;
    }


    //---------------------------------------------------------------------------
    TimeSourceTask_i::~TimeSourceTask_i()
    {
      cout << "TimeSourceTask_i::~TimeSourceTask_i[ " << this << " ]" << endl;
    }


    //---------------------------------------------------------------------------
    void TimeSourceTask_i::prepare()
    {
      cout << "TimeSourceTask_i::prepare[ " << this << " ]" << endl;
      
      this->m_next_i.init( true );
      this->m_delta_i.init( this->m_delta_i() );
    }
    
    
    //---------------------------------------------------------------------------
    bool TimeSourceTask_i::step()
    {
      cout << "TimeSourceTask_i::step[ " << this << " ]" << endl;
      
      this->m_next_i.retrieve();
      this->m_delta_i.retrieve();

      if ( this->m_next_i() ) 
      {
        this->increment();
      }

      dimensionedScalar time_eps( this->deltaT() * 0.01 );
      bool is_time_exit = this->value() - time_eps > this->endTime();
      this->m_finished_o.publish( is_time_exit && this->m_next_i() );

      MSG( "TimeSourceTask_i]::step[ " << this 
           << " | value = " << this->value()
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
    void TimeSourceTask_i::increment()
    {
      this->setTime( this->value() + this->deltaT(), this->timeIndex() + 1 );
    }
    

    //---------------------------------------------------------------------------
    void TimeSourceTask_i::setTime( dimensionedScalar newTime, label newIndex )
    {
      this->m_time_o() = newTime;
      this->m_index_o() = newIndex;
    }

    dimensionedScalar TimeSourceTask_i::value()
    {
      return this->m_time_o().value();
    }

    label TimeSourceTask_i::timeIndex()
    {
      return this->m_index_o();
    }
    

    //---------------------------------------------------------------------------
    void TimeSourceTask_i::setDeltaT( dimensionedScalar deltaT )
    {
      this->m_delta_i() = deltaT;
    }
    
    dimensionedScalar TimeSourceTask_i::deltaT()
    {
      return this->m_delta_i().value();
    }
    

    //---------------------------------------------------------------------------
    void TimeSourceTask_i::setEndTime( dimensionedScalar endTime )
    {
      this->m_end = endTime;
    }

    dimensionedScalar TimeSourceTask_i::endTime()
    {
      return this->m_end.value();
    }


    //---------------------------------------------------------------------------
    void TimeSourceTask_i::setWriteInterval( label writeInterval )
    {
      this->m_writeInterval = writeInterval;
    }

    label TimeSourceTask_i::getWriteInterval()
    {
      return this->m_writeInterval;
    }
    

    //---------------------------------------------------------------------------
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
