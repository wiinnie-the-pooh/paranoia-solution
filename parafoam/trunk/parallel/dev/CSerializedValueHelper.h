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
#ifndef dev_CSerializedValueHelper_h
#define dev_CSerializedValueHelper_h


//---------------------------------------------------------------------------
#include "parallel/dev/CSimpleValueHelper.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <sstream>
#include <string>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class ValueHelperType >
    void save_value( const ValueHelperType& the_value_helper, typename std::string& the_data )
    {
      typename std::ostringstream os;
      typename boost::archive::text_oarchive oa( os );
      
      oa << the_value_helper;
      
      the_data = os.str();
    }


    //---------------------------------------------------------------------------
    template< class ValueHelperType >
    void restore_value( const typename std::string& the_data, ValueHelperType& the_value_helper )
    {
      typename std::istringstream is( the_data );
      
      typename boost::archive::text_iarchive ia( is );
      
      ia >> the_value_helper;
    }


    //---------------------------------------------------------------------------
    template< class Type >
    struct CSerializedValueHelper : CSimpleValueHelperBase< Type >
    {
      typedef typename CSimpleValueHelperBase< Type >::TValue TValue;

      CSerializedValueHelper( const TValue& the_value = TValue() )
        : CSimpleValueHelperBase< Type >( the_value )
      {}
        
      CSerializedValueHelper( const CSerializedValueHelper& the_value_helper )
        : CSimpleValueHelperBase< Type >( the_value_helper.value )
      {}

      template< class ArchiveType >
      void serialize( ArchiveType & ar, const unsigned int /* revision_number */ )
      {
        ar & this->value;
      }

      template< class SerializedDataHolderType >
      void save_data( SerializedDataHolderType& the_serialized_data_holder ) const
      {
        save_value( *this, the_serialized_data_holder() );
      }

      template< class SerializedDataHolderType >
      void restore_data( const SerializedDataHolderType& the_serialized_data_holder )
      {
        restore_value( the_serialized_data_holder(), *this );
      }
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
