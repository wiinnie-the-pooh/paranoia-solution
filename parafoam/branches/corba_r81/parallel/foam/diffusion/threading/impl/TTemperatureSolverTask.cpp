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
#include "parallel/foam/diffusion/threading/impl/TTemperatureSolverTask.h"

#include "parallel/foam/diffusion/threading/impl/TTemperatureSolver.h"

#include "parallel/foam/common/SFoamMutex.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    TTemperatureSolverTask::TPtr 
    TTemperatureSolverTask::create( const argList& args, const Switch& isTransient )
    {
      return new TTemperatureSolverTask( TTemperatureSolver::create( args, isTransient ) );
    }


    //-----------------------------------------------------------------------
    TTemperatureSolverTask::TTemperatureSolverTask( const TTemperatureSolverPtr& engine )
      : CSolverTaskBase( *engine->runTime )
      , engine( engine )
      , m_powerDensity_i( "powerDensity", true, *this )
      , m_fvMesh_o( "fvMesh", false, *this )
      , m_Tfuel_o( "Tfuel", false, *this )
      , m_Tmod_o( "Tmod", false, *this )
    {}


    //---------------------------------------------------------------------------
    TTemperatureSolverTask::~TTemperatureSolverTask()
    {}


    void TTemperatureSolverTask::init()
    {
      MSG( "Start of TTemperatureSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::init();
      
      this->m_powerDensity_i.init( clone( this->engine->powerDensity() ) );
      
      this->m_fvMesh_o.publish( this->engine->mesh );
    }


    //---------------------------------------------------------------------------
    bool TTemperatureSolverTask::step()
    {      
      MSG( "TTemperatureSolverTask[ " << this << " ]::step entered\n" );

      // It is necessary to retrieve the fileds before time modification
      // ( which take place in "pre_step" function)
      this->m_powerDensity_i.retrieve( this->engine->mesh );
      MSG( "TTemperatureSolverTask[ " << this << " ]::data retrieved\n" );
      
      if ( this->pre_step() )
      {
        //SFoamMutex aMutex; // pure matrix manipulations seems are threadsafe

        scalar residual = this->engine->solve();
        
        MSG( "TTemperatureSolverTask[ " << this << " ]"
             << " | " << this->runTime.timeName().c_str()
             << " | " << residual << "\n" );
      
        this->m_residual_o.publish( residual );

        this->m_Tfuel_o.publish( clone( this->engine->Tfuel() ) );
        this->m_Tmod_o.publish( clone( this->engine->Tmod() ) );

        MSG( "TTemperatureSolverTask[ " << this << " ]::data published\n" );
      }

      return this->post_step();
    }


    //---------------------------------------------------------------------------
    void TTemperatureSolverTask::destroy()
    {
      MSG( "End of TTemperatureSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::destroy();
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
