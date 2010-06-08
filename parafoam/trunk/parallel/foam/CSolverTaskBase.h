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
#ifndef foam_CSolverTaskBase_h
#define foam_CSolverTaskBase_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"

#include "parallel/dev/CPortHelper.h"

#include "parallel/dev/CBoolPort.h"

#include "parallel/dev/CIntPort.h"
#include "parallel/dev/CDoublePort.h"

#include "parallel/foam/CDimensionedScalar.h"
#include "parallel/foam/CSerializedDimensionedScalar.h"

#include <Time.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    using namespace Foam;


    //---------------------------------------------------------------------------
    struct CSolverTaskBase : dev::TTask
    {
      typedef base::SmartPtrDef< CSolverTaskBase >::type TPtr;
 
      CSolverTaskBase( Time& runTime );

      ~CSolverTaskBase();

    protected:
      Foam::Time& runTime;

      dev::CPortHelper< CSerializedDimensionedScalar > m_time_i;
      dev::CPortHelper< dev::CIntPort > m_index_i;
      dev::CPortHelper< dev::CBoolPort > m_write_i;
      dev::CPortHelper< dev::CBoolPort > m_stop_i;

      dev::CPortHelper< dev::CBoolPort > m_finished_o;
      dev::CPortHelper< dev::CDoublePort > m_residual_o;

    protected:
      virtual void init();
      virtual void destroy();

      bool pre_step();
      bool post_step();

      bool finished();
    };


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
