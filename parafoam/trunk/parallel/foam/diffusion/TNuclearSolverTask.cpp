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
#include "parallel/foam/diffusion/TNuclearSolverTask.h"

#include "parallel/foam/diffusion/TNuclearSolver.h"

#include "parallel/foam/SFoamMutex.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    TNuclearSolverTask::TPtr 
    TNuclearSolverTask::create( const argList& args, const Switch& isTransient )
    {
      return new TNuclearSolverTask( TNuclearSolver::create( args, isTransient ) );
    }


    //-----------------------------------------------------------------------
    TNuclearSolverTask::TNuclearSolverTask( const TNuclearSolverPtr& engine )
      : CSolverTaskBase( *engine->runTime )
      , engine( engine )
      , m_Tfuel_i( "Tfuel", true, *this )
      , m_Tmod_i( "Tmod", true, *this )
      , m_fvMesh_o( "fvMesh", false, *this )
      , m_powerDensity_o( "powerDensity", false, *this )
    {}


    //---------------------------------------------------------------------------
    TNuclearSolverTask::~TNuclearSolverTask()
    {}


    //---------------------------------------------------------------------------
    void TNuclearSolverTask::init()
    {
      MSG( "Start of TNuclearSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::init();

      this->m_Tfuel_i.init( clone( this->engine->Tfuel() ) );
      this->m_Tmod_i.init( clone( this->engine->Tmod() ) );
      
      this->m_fvMesh_o.publish( this->engine->mesh );
    }
    

    //---------------------------------------------------------------------------
    bool TNuclearSolverTask::step()
    {
      MSG( "TNuclearSolverTask[ " << this << " ]::step entered\n" );

      // It is necessary to retrieve the fileds before time modification
      // ( which take place in "pre_step" function)
      this->m_Tfuel_i.retrieve( this->engine->mesh );
      this->m_Tmod_i.retrieve( this->engine->mesh );
      MSG( "TNuclearSolverTask[ " << this << " ]::data retrieved\n" );
      
      if ( this->pre_step() )
      {
        //SFoamMutex aMutex; // pure matrix manipulations seems are threadsafe

        scalar residual = this->engine->solve();
        
        MSG( "TNuclearSolverTask[ " << this << " ]"
	     << " | " << this->runTime.timeName().c_str()
	     << " | " << residual << "\n" );
      
        this->m_residual_o.publish( residual );

        this->m_powerDensity_o.publish( clone( this->engine->powerDensity() ) );
        MSG( "TNuclearSolverTask[ " << this << " ]::data published\n" );

      }

      return this->post_step();
    }

    
    //---------------------------------------------------------------------------
    void TNuclearSolverTask::destroy()
    {
      MSG( "End of TNuclearSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::destroy();
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
