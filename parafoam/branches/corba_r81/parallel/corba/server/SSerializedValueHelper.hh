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
#ifndef corba_server_SSerializedValueHelper_hh
#define corba_server_SSerializedValueHelper_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/SSimpleValueHelper.hh"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <CORBA.h>

#include <sstream>
#include <string>


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  template< class Type >
  struct SSerializedValueHelper : SSimpleValueHelperBase< Type >
  {
    typedef typename SSimpleValueHelperBase< Type >::TValue TValue;

    SSerializedValueHelper( const TValue& the_value = TValue() )
      : SSimpleValueHelperBase< Type >( the_value )
    {}
    
    SSerializedValueHelper( const SSerializedValueHelper& the_value_helper )
      : SSimpleValueHelperBase< Type >( the_value_helper.value )
    {}
    
    SSerializedValueHelper( TRawData* the_serialized_data )
    {
      TRawData_var a_serialized_data( the_serialized_data );
      typename std::istringstream is( std::string( ( char* ) &a_serialized_data[ 0 ], a_serialized_data->length() ) );
    
      typename boost::archive::text_iarchive ia( is );
    
      ia >> *this;
    }
    
    operator TRawData* () const
    {
      typename std::ostringstream os;
      typename boost::archive::text_oarchive oa( os );
    
      oa << *this;
    
      std::string a_string = os.str();

      TRawData a_serialized_data( a_string.size(), a_string.size(), ( CORBA::Octet* ) &a_string[ 0 ] );

      return new TRawData( a_serialized_data );
    }
    
    template< class ArchiveType >
    void serialize( ArchiveType & ar, const unsigned int /* revision_number */ )
    {
      ar & this->value;
    }
  };
  
  
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
