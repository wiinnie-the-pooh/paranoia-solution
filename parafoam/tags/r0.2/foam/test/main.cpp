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
#include "parallel/foam/CSerializedFvMesh.h"

#include "iostream"


//---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
  using namespace Foam;

# include "setRootCase.H"

  using namespace parallel::foam;

  TimePtr runTime = createTime( args.path(), "nuclear" );

  fvMeshPtr mesh = createMesh( *runTime );

  Foam::Info << "mesh = " << mesh->time().path() << Foam::nl;

  CSerializedFvMesh::TDataHolder aDataHolder( mesh );
      
  std::cout << "aDataHolder.store = " << aDataHolder() << std::endl;

  std::cout << "aDataHolder.restore = " << aDataHolder.value()->time().path() << std::endl;

  std::cout << "End" << std::endl;

  return 0;
}


//---------------------------------------------------------------------------