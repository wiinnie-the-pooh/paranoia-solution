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
#ifndef foam_CSerializedGeometricalFieldValueHelper_h
#define foam_CSerializedGeometricalFieldValueHelper_h


//---------------------------------------------------------------------------
#include "parallel/foam/CTmpValueHelper.h"

#include "parallel/dev/CSerializedValueHelper.h"

#include <loki/SmartPtr.h>

#include <fvMesh.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    template< class Type >
    struct CSerializedGeometricalFieldValueHelper : CTmpValueHelper< Type >
    {
      typedef typename CTmpValueHelper< Type >::TValue TValue;

      typedef typename Loki::SmartPtrDef< Foam::fvMesh >::type TMesh;
      TMesh mesh;

      CSerializedGeometricalFieldHelper()
      {}
            
      CSerializedGeometricalFieldValueHelper( const TValue& the_value )
        : CTmpValueHelper< Type >( the_value )
      {}

      CSerializedGeometricalFieldValueHelper( const Type& the_value )
        : CTmpValueHelper< Type >( the_value )
      {}

      CSerializedGeometricalFieldValueHelper( const TMesh& the_mesh )
        : CTmpValueHelper< Type >( the_mesh )
      {}

      CSerializedGeometricalFieldValueHelper( const CSerializedGeometricalFieldValueHelper& the_value_helper )
        : CTmpValueHelper< Type >( the_value_helper.value )
      {}
        
      virtual ~CSerializedGeometricalFieldValueHelper()
      {}
            
      template< class SerializedDataHolderType >
      void save_data( SerializedDataHolderType& the_serialized_data_holder ) const
      {
        dev::save_value( *this, the_serialized_data_holder() );
      }

      template< class SerializedDataHolderType >
      void restore_data( const SerializedDataHolderType& the_serialized_data_holder )
      {
        dev::restore_value( the_serialized_data_holder(), *this );
      }

      template< class ArchiveType >
      void save( ArchiveType & ar, const unsigned int /* revision_number */ ) const
      {
        Foam::OStringStream an_ostream;

        an_ostream << this->value->name();

        an_ostream << this->value();

        std::string a_string = an_ostream.str();

        ar << a_string;
      }

      template< class ArchiveType >
      void load( ArchiveType & ar, const unsigned int /* revision_number */ )
      {
        std::string a_name;
        ar >> a_name;

        std::string a_string;
        ar >> a_string;

        Foam::IStringStream an_istream( a_string );
        
        this->value = new Type( Foam::IOobject( a_name,
                                                Foam::fileName( this->mesh->time().timeName() ),
                                                * this->mesh,
                                                Foam::IOobject::NO_READ,
                                                Foam::IOobject::AUTO_WRITE ),
                                * this->mesh,
                                an_istream );
      }

      template< class ArchiveType >
      void serialize( ArchiveType & ar, const unsigned int revision_number )
      {
        boost::serialization::split_member( ar, *this, revision_number );
      }
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
