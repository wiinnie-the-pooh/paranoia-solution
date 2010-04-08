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
#ifndef CVolFieldMapperTask_h
#define CVolFieldMapperTask_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"

#include "parallel/dev/CBoolPort.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/foam/CFvMeshPort.h"

#include "parallel/foam/CGeometricFieldPortHelper.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    using namespace Foam;


    //-----------------------------------------------------------------------
    struct CVolFieldMapperTask : dev::TTask
    {
      typedef base::SmartPtrDef< CVolFieldMapperTask >::type TPtr;

      CVolFieldMapperTask();
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
      dev::CPortHelper< dev::CBoolPort > m_stop_i;

      CGeometricFieldPortHelper< Foam::volScalarField > m_SourceField_i;
      dev::CPortHelper< CFvMeshPort > m_SourceMesh_i;
      dev::CPortHelper< CFvMeshPort > m_TargetMesh_i;

      CGeometricFieldPortHelper< Foam::volScalarField > m_TargetField_o;
      dev::CPortHelper< dev::CBoolPort > m_finished_o;
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
