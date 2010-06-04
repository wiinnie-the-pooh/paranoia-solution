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
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/foam/utilities.h"

#include "parallel/foam/SFoamMutex.h"


//---------------------------------------------------------------------------
namespace parallel 
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    TimePtr createTime( const fileName& rootPath, const fileName& caseName )
    {
      SFoamMutex aMutex;

      return new Time( Time::controlDictName,
                       rootPath,
                       caseName );
    }


    //-----------------------------------------------------------------------
    fvMeshPtr createMesh( const Time& runTime )
    {
      SFoamMutex aMutex;

      return new fvMesh( IOobject( fvMesh::defaultRegion,
                                   fileName( runTime.timeName() ),
                                   runTime,
                                   IOobject::MUST_READ ) );
    }
    

    //-----------------------------------------------------------------------
    tmp< volScalarField > clone( const volScalarField& theValue )
    {
      SFoamMutex aMutex;

      return tmp< volScalarField >( new volScalarField( theValue ) );
    }
    

    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
