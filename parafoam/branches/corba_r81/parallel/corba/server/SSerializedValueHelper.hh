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
#ifndef dev_SSerializedValueHelper_hh
#define dev_SSerializedValueHelper_hh


//---------------------------------------------------------------------------
#include "parallel/dev/CSimpleValueHelper.h"

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
  struct SSerializedValueHelper : dev::CSimpleValueHelperBase< Type >
  {
    typedef typename dev::CSimpleValueHelperBase< Type >::TValue TValue;

    SSerializedValueHelper( const TValue& the_value = TValue() )
      : dev::CSimpleValueHelperBase< Type >( the_value )
    {}
    
    SSerializedValueHelper( const SSerializedValueHelper& the_value_helper )
      : dev::CSimpleValueHelperBase< Type >( the_value_helper.value )
    {}
    
    template< class SerializedDataHolderVarType >
    SSerializedValueHelper& operator = ( const SerializedDataHolderVarType& the_data_holder )
    {
      CORBA::String_var a_serialized_data = the_data_holder->value();

      typename std::istringstream is( a_serialized_data );
    
      typename boost::archive::text_iarchive ia( is );
    
      ia >> *this;

      return *this;
    }
    
    operator std::string () const
    {
      typename std::ostringstream os;
      typename boost::archive::text_oarchive oa( os );
    
      oa << *this;
    
      return os.str();
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
