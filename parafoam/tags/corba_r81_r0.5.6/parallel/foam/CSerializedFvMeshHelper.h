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
#ifndef dev_CSerializedFvMeshHelper_h
#define dev_CSerializedFvMeshHelper_h


//---------------------------------------------------------------------------
#include "parallel/dev/CSerializedValueHelper.h"

#include "parallel/dev/CSmartPtrValueHelper.h"

#include "parallel/foam/utilities.h"

#include "parallel/foam/SFoamMutex.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    struct CSerializedFvMeshHelper : dev::CSmartPtrValueHelper< Foam::fvMesh >
    {
      TimePtr runTime;

      CSerializedFvMeshHelper( const TValue& the_value = TValue() )
        : dev::CSmartPtrValueHelper< Foam::fvMesh >( the_value )
      {}

      CSerializedFvMeshHelper( const CSerializedFvMeshHelper& the_value_helper )
        : dev::CSmartPtrValueHelper< Foam::fvMesh >( the_value_helper.value )
      {}

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

      template< class ArchiveType >
      void save( ArchiveType & ar, const unsigned int /* revision_number */ ) const
      {
        SFoamMutex aMutex;

        Foam::OStringStream an_ostream;

        an_ostream << this->value->time().rootPath() << Foam::token::SPACE
                   << this->value->time().caseName();

        std::string a_string = an_ostream.str();

        ar << a_string;
      }

      template< class ArchiveType >
      void load( ArchiveType & ar, const unsigned int /* revision_number */ )
      {
        SFoamMutex aMutex;

        std::string a_string;

        ar >> a_string;

        Foam::IStringStream an_ostream( a_string );

        Foam::fileName rootPath;
        an_ostream >> rootPath;

        Foam::fileName caseName;
        an_ostream >> caseName;

        this->runTime = createTime( rootPath, caseName );

        this->value = createMesh( *this->runTime );
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
