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
#ifndef corba_server_Foam_DimensionedScalarValueHelper_hh
#define corba_server_Foam_DimensionedScalarValueHelper_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/SSerializedValueHelper.hh"

#include <dimensionedScalar.H>

#include <IStringStream.H>
#include <OStringStream.H>


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  template<>
  struct SSerializedValueHelper< Foam::dimensionedScalar > : SSimpleValueHelperBase< Foam::dimensionedScalar >
  {
    SSerializedValueHelper( const TValue& the_value = Foam::dimensionedScalar( 0.0 ) )
      : SSimpleValueHelperBase< Foam::dimensionedScalar >( the_value )
      {}

    SSerializedValueHelper( const SSerializedValueHelper& the_value_helper )
      : SSimpleValueHelperBase< Foam::dimensionedScalar >( the_value_helper.value )
    {}

    SSerializedValueHelper( const char* the_serialized_data )
      : SSimpleValueHelperBase< Foam::dimensionedScalar >( Foam::dimensionedScalar( 0.0 ) )
    {
      std::istringstream is( the_serialized_data );
    
      boost::archive::text_iarchive ia( is );
    
      ia >> *this;
    }
    
    operator std::string () const
    {
      std::ostringstream os;
      boost::archive::text_oarchive oa( os );
    
      oa << *this;
    
      return os.str();
    }
    
    template< class ArchiveType >
    void save( ArchiveType & ar, const unsigned int /* revision_number */ ) const
    {
      Foam::OStringStream an_ostream;
      
      an_ostream << /* this->value.name() */ "dummy_name" << Foam::token::SPACE 
		 << this->value.dimensions() << Foam::token::SPACE
		 << this->value.value();
      
      std::string a_string = an_ostream.str();
      
      ar << a_string;
    }
    
    template< class ArchiveType >
    void load( ArchiveType & ar, const unsigned int /* revision_number */ )
    {
      std::string a_string;
      
      ar >> a_string;
      
      Foam::IStringStream an_ostream( a_string );
      
      this->value = TValue( an_ostream );
    }
    
    template< class ArchiveType >
    void serialize( ArchiveType & ar, const unsigned int revision_number )
    {
      boost::serialization::split_member( ar, *this, revision_number );
    }
  };
  
  
  //-----------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
