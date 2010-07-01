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
#include "parallel/corba/foam/server/Foam_SolverBaseTaskFactory_i.hh"

#include "parallel/corba/common/FactoryLauncher.hh"


//---------------------------------------------------------------------------
int main( int argc, char** argv )
{
  using namespace parallel;
  using namespace parallel::foam;

  return run< SolverBaseTaskFactory_i, SolverBaseTaskFactory >( argc, argv, "TaskFactory", "Foam_SolverBase" );
}


//---------------------------------------------------------------------------
