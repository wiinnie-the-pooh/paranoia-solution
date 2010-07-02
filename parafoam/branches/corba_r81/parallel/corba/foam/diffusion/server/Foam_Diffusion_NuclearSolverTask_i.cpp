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
#include "parallel/corba/foam/diffusion/server/Foam_Diffusion_NuclearSolverTask_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  namespace foam
  {
    //---------------------------------------------------------------------------
    namespace diffusion
    {
      //---------------------------------------------------------------------------
      NuclearSolverTask_i::NuclearSolverTask_i( const CORBA::ORB_var& theORB, 
						const PortableServer::POA_var& thePOA )
	: TransientObject_i( theORB, thePOA )
	, TaskBase_i( theORB, thePOA )
	, SolverBaseTask_i( theORB, thePOA )
      {
	TMSG( "NuclearSolverTask_i::NuclearSolverTask_i[ " << this << " ]\n" );
      }
      
      
      //---------------------------------------------------------------------------
      NuclearSolverTask_i::~NuclearSolverTask_i()
      {
	TMSG( "NuclearSolverTask_i::~NuclearSolverTask_i[ " << this << " ]\n" );
      }
      
      
      //---------------------------------------------------------------------------
      void NuclearSolverTask_i::init( const char* theCasePath, ::CORBA::Boolean theIsTransient )
      {
	TMSG( "NuclearSolverTask_i::init[ " << this << " ]\n" );
      }
      

      //---------------------------------------------------------------------------
      void NuclearSolverTask_i::prepare()
      {
	TMSG( "NuclearSolverTask_i::prepare[ " << this << " ]\n" );
      }
      

      //---------------------------------------------------------------------------
      bool NuclearSolverTask_i::step()
      {
	TMSG( "NuclearSolverTask_i::step[ " << this << " ]\n" );
	
	this->pre_step();
	
	return this->post_step();
      }
      
      
      //---------------------------------------------------------------------------
    }
      

    //---------------------------------------------------------------------------
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
