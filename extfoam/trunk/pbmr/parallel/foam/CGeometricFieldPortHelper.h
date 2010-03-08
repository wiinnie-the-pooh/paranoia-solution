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
#ifndef dev_CGeometricFieldPortHelper_h
#define dev_CGeometricFieldPortHelper_h


//---------------------------------------------------------------------------
#include "parallel/dev/CPortHelperBase.h"

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
