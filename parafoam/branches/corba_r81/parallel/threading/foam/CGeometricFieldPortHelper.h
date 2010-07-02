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
#ifndef dev_CGeometricFieldPortHelper_h
#define dev_CGeometricFieldPortHelper_h


//---------------------------------------------------------------------------
#include "parallel/threading/dev/CPortHelperBase.h"

#include "parallel/foam/CSerializedGeometricalField.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    template< class Type >
    struct CGeometricFieldPortHelper : dev::CPortHelperBase< CSerializedGeometricalField< Type > >
    {
      typedef CSerializedGeometricalField< Type > PortType;

      typedef typename dev::CPortHelperBase< PortType >::TDataHolder TDataHolder;
      typedef typename dev::CPortHelperBase< PortType >::TDataHolderPtr TDataHolderPtr;

      typedef typename dev::CPortHelperBase< PortType >::TValueHelper TValueHelper;
      typedef typename TValueHelper::TMesh TMesh;

      CGeometricFieldPortHelper( const std::string& theName, bool theIsInput, dev::TTask& theTask )
        : dev::CPortHelperBase< PortType >( theName, theIsInput, theTask )
      {}

      //---------------------------------------------------------------------------
      TValueHelper retrieve( const TMesh& the_mesh )
      {
        TDataHolderPtr a_data_holder = this->template __retrieve__< TDataHolder >( this->task(), this->name() );

        this->value_helper = a_data_holder->value( the_mesh );

        return this->value_helper;
      }
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
