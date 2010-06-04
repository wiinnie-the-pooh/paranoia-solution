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
#include "parallel/dev/TTaskMgr.h"

#include "parallel/toys/SolverExamples.h"
#include "parallel/toys/TVisualizer.h"

#include <iostream>

using parallel::base::TTaskMgrPtr;
using parallel::base::TTaskPtr;

using parallel::dev::TTaskMgr;
using namespace parallel::toys;


//---------------------------------------------------------------------------
int main1( int argc, char *argv[] )
{
  TTaskMgrPtr aMgr = TTaskMgr::get_instance();

  TTaskPtr a( new DataSource( 200 ) );
  TTaskPtr b( new Copy( 100 ) );
  TTaskPtr c( new Palindrome( 100 ) );
  TTaskPtr d( new Concat( 50 ) );

  TTaskPtr v1( new StrVis() );
  TTaskPtr v2( new StrVis() );
  TTaskPtr v3( new StrVis() );

  aMgr->add_task( a );
  aMgr->add_task( b );
  aMgr->add_task( c );
  aMgr->add_task( d );
  aMgr->add_task( v1 );
  aMgr->add_task( v2 );
  aMgr->add_task( v3 );

  aMgr->connect( a, "res", b, "arg" );
  aMgr->connect( a, "res", c, "arg" );
  aMgr->connect( b, "res", d, "arg1" );
  aMgr->connect( c, "res", d, "arg2" );

  aMgr->connect( c, "res", v1, "arg" );
  aMgr->connect( d, "res", v2, "arg" );
  aMgr->connect( b, "res", v3, "arg" );

  std::cout << "Start" << std::endl;
  aMgr->start();
  sleep( 5 );
  std::cout << "Pause" << std::endl;
  aMgr->pause();
  sleep( 2 );
  std::cout << "Resume" << std::endl;
  aMgr->resume();
  sleep( 5 );
  std::cout << "Stop" << std::endl;
  aMgr->stop();

  return 0;
}


//---------------------------------------------------------------------------
int main2( int argc, char *argv[] )
{
  TTaskMgrPtr aMgr = TTaskMgr::get_instance();

  TTaskPtr a( new PlusX( 200, 1, 1 ) );
  TTaskPtr b( new MultX( 100, 1, 2 ) );

  TTaskPtr v1( new DoubleVis( "a" ) );
  TTaskPtr v2( new DoubleVis( "b" ) );

  aMgr->add_task( a );
  aMgr->add_task( b );
  aMgr->add_task( v1 );
  aMgr->add_task( v2 );

  aMgr->connect( a, "res", b, "arg" );
  aMgr->connect( b, "res", a, "arg" );

  aMgr->connect( a, "res", v1, "arg" );
  aMgr->connect( b, "res", v2, "arg" );

  std::cout << "Start" << std::endl;
  aMgr->start();
  sleep( 2 );
  std::cout << "Pause" << std::endl;
  aMgr->pause();
  sleep( 5 );
  std::cout << "Resume" << std::endl;
  aMgr->resume();
  sleep( 2 );
  std::cout << "Stop" << std::endl;
  aMgr->stop();

  return 0;
}


//---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    return main2( argc, argv );
}


//---------------------------------------------------------------------------
