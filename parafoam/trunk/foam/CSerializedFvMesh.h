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
#ifndef foam_CSerializedFvMesh_h
#define foam_CSerializedFvMesh_h


//---------------------------------------------------------------------------
#include "parallel/dev/TPort.h"

#include "parallel/dev/TTask.h"

#include "parallel/dev/CSerializedDataHolder.h"

#include "parallel/foam/CSerializedFvMeshHelper.h"


//---------------------------------------------------------------------------
namespace parallel 
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    struct CSerializedFvMesh : dev::TPort
    {
      PARALLEL_DERIVED_PORT_DEF( CSerializedFvMesh );
    
      typedef CSerializedFvMeshHelper TValueHelper;

      struct TDataHolder : dev::CSerializedDataHolder< TValueHelper >
      {
        TDataHolder( const TValueHelper& the_value_helper )
          : dev::CSerializedDataHolder< TValueHelper >( the_value_helper )
        {}
      };
    };
    

    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
