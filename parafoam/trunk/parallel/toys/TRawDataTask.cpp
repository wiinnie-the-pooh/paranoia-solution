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
#include "parallel/toys/TRawDataTask.h"
#include "parallel/dev/TPort.h"


//---------------------------------------------------------------------------
namespace
{
  using namespace parallel;

  //-----------------------------------------------------------------------
  struct CRawDataPort : dev::TPort
  {
    PARALLEL_DERIVED_PORT_DEF( CRawDataPort );
  };


  //-----------------------------------------------------------------------
  base::TPortPtr
  find_or_create_port( const base::TName2Port& theName2Port, 
                       const std::string& theName,
                       dev::TTask& theTask )
  {
    base::TName2Port::const_iterator anIter = theName2Port.find( theName );
    if ( anIter != theName2Port.end() )
      return anIter->second;

    return base::TPortPtr( new CRawDataPort( theName, theTask ) );
  }


  //-----------------------------------------------------------------------
}


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    base::TPortPtr TRawDataTask::get_input_port( const std::string& theName )
    {
      return find_or_create_port( this->get_input_ports(), theName, *this );
    }


    //---------------------------------------------------------------------------
    base::TPortPtr TRawDataTask::get_output_port( const std::string& theName )
    {
      return find_or_create_port( this->get_output_ports(), theName, *this );
    }


    //---------------------------------------------------------------------------
    TFieldPtr TRawDataTask::read( const std::string& theName )
    {
      return this->retrieve< TField >( theName );
    }


    //---------------------------------------------------------------------------
    void TRawDataTask::write( const std::string& theName, TFieldPtr theField )
    {
      this->publish( theName, theField );
    }
    

    //---------------------------------------------------------------------------
    void TRawDataTask::write_raw( const std::string& theName, TData theData, int theSize )
    {
      this->write( theName, TFieldPtr( new TField( theName, theData, theSize ) ) );
    }
    
    
    //---------------------------------------------------------------------------
  }
}

//---------------------------------------------------------------------------
