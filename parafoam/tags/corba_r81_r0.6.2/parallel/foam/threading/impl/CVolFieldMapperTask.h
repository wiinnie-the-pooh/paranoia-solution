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
#ifndef CVolFieldMapperTask_h
#define CVolFieldMapperTask_h


//---------------------------------------------------------------------------
#include "parallel/threading/dev/TTask.h"

#include "parallel/threading/dev/CBoolPort.h"

#include "parallel/threading/dev/CPortHelper.h"

#include "parallel/foam/threading/impl/CSerializedFvMesh.h"

#include "parallel/foam/threading/impl/CGeometricFieldPortHelper.h"


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
      dev::CPortHelper< CSerializedFvMesh > m_SourceMesh_i;
      dev::CPortHelper< CSerializedFvMesh > m_TargetMesh_i;

      CGeometricFieldPortHelper< Foam::volScalarField > m_TargetField_o;
      dev::CPortHelper< dev::CBoolPort > m_finished_o;
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
