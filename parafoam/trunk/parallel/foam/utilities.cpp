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
#include "parallel/foam/utilities.h"

#include <pthread.h>


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
    static pthread_mutex_t FOAM_MUTEX;

    int init_foam_mutex()
    {
      pthread_mutexattr_t attr;
      pthread_mutexattr_init( &attr );
      pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
      pthread_mutex_init( &FOAM_MUTEX, &attr );
      
      return 1;
    }

    static int INIT_FOAM_MUTEX = init_foam_mutex();


    //-----------------------------------------------------------------------
    SFoamMutex::SFoamMutex()
    {
      pthread_mutex_lock( &FOAM_MUTEX );
    }

    SFoamMutex::~SFoamMutex()
    {
      pthread_mutex_unlock( &FOAM_MUTEX );
    }


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
