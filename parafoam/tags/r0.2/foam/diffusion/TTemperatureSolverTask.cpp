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
#include "parallel/foam/diffusion/TTemperatureSolverTask.h"

#include "parallel/foam/diffusion/TTemperatureSolver.h"

#include "parallel/dev/TPort.h"

#include "parallel/dev/CBoolPort.h"


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
      MSG( "\nStart of TTemperatureSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::init();
      
      this->m_powerDensity_i.init( clone( this->engine->powerDensity() ) );
      
      this->m_fvMesh_o.publish( this->engine->mesh );
    }


    //---------------------------------------------------------------------------
    bool TTemperatureSolverTask::step()
    {      
      MSG( "\nTTemperatureSolverTask[ " << this << " ]::step\n" );

      // It is necessary to retrieve the fileds before time modification
      // ( which take place in "pre_step" function)
      this->m_powerDensity_i.retrieve( this->engine->mesh );
      
      if ( this->pre_step() )
      {
        scalar residual = this->engine->solve();
        
        MSG( "\nTTemperatureSolverTask[ " << this << " ]"
	     << " | " << this->runTime.timeName().c_str()
	     << " | " << residual << "\n" );
      
        this->m_residual_o.publish( residual );

        this->m_Tfuel_o.publish( clone( this->engine->Tfuel() ) );
        this->m_Tmod_o.publish( clone( this->engine->Tmod() ) );
      }

      return this->post_step();
    }


    //---------------------------------------------------------------------------
    void TTemperatureSolverTask::destroy()
    {
      MSG( "\nEnd of TTemperatureSolverTask[ " << this << " ]\n" );

      CSolverTaskBase::destroy();
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
