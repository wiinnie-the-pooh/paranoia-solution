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
#include "parallel/dev/TTaskMgr.h"

#include "parallel/foam/diffusion/TNuclearSolverTask.h"
#include "parallel/foam/diffusion/TTemperatureSolverTask.h"

#include "parallel/foam/CVolFieldMapperTask.h"
#include "parallel/foam/CTimeSourceTask.h"

#include "parallel/dev/CORPredicateTask.h"
#include "parallel/dev/CConvergencyCriteriaTask.h"

#include <iostream>

using parallel::base::TTaskMgrPtr;
using parallel::base::TTaskPtr;

using parallel::dev::TTaskMgr;
using parallel::dev::CORPredicateTask;
using parallel::dev::CConvergencyCriteriaTask;

using namespace parallel::foam;


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    void connect( const TTaskMgrPtr& theTaskMgr, 
                  const CSolverTaskBase::TPtr& theSolverBase, 
                  const CConvergencyCriteriaTask::TPtr& theConvergencyCriteria )
    {
      theTaskMgr->connect( theSolverBase, "residual", theConvergencyCriteria, "residual" );
      theTaskMgr->connect( theSolverBase, "finished", theConvergencyCriteria, "continue" );
   }


    //-----------------------------------------------------------------------
    void connect( const TTaskMgrPtr& theTaskMgr, 
                  const CConvergencyCriteriaTask::TPtr& theLeftCriteria, 
                  const CConvergencyCriteriaTask::TPtr& theRighCriteria, 
                  const CORPredicateTask::TPtr& theCombinedCriteria )
    {
      theTaskMgr->connect( theLeftCriteria, "next", theCombinedCriteria, "left" );
      theTaskMgr->connect( theLeftCriteria, "finished", theCombinedCriteria, "continue_left" );

      theTaskMgr->connect( theRighCriteria, "next", theCombinedCriteria, "right" );
      theTaskMgr->connect( theLeftCriteria, "finished", theCombinedCriteria, "continue_right" );
   }


    //-----------------------------------------------------------------------
    void connect( const TTaskMgrPtr& theTaskMgr, 
                  const CTimeSourceTask::TPtr& theTimeSource, 
                  const CSolverTaskBase::TPtr& theSolverBase )
    {
      theTaskMgr->connect( theTimeSource, "time", theSolverBase, "time" );
      theTaskMgr->connect( theTimeSource, "index", theSolverBase, "index" );
      theTaskMgr->connect( theTimeSource, "next", theSolverBase, "next" );
      theTaskMgr->connect( theTimeSource, "stop", theSolverBase, "stop" );
      theTaskMgr->connect( theTimeSource, "write", theSolverBase, "write" );
    }
    
    
    //-----------------------------------------------------------------------
    void connect( const TTaskMgrPtr& theTaskMgr, 
                  const CSolverTaskBase::TPtr& theSourceTask, 
                  const std::string thePortName,
                  const CSolverTaskBase::TPtr& theTragetTask )
    {
      TTaskPtr aMapperTask( new CVolFieldMapperTask() );
      theTaskMgr->add_task( aMapperTask );

      theTaskMgr->connect( theSourceTask, "finished", aMapperTask, "stop" );
      theTaskMgr->connect( aMapperTask, "finished", theSourceTask, "continue" );

      theTaskMgr->connect( theSourceTask, thePortName, aMapperTask, "theSourceField" );
      theTaskMgr->connect( theSourceTask, "fvMesh", aMapperTask, "theSourceMesh" );
      theTaskMgr->connect( theTragetTask, "fvMesh", aMapperTask, "theTargetMesh" );
      theTaskMgr->connect( aMapperTask, "aTargetField", theTragetTask, thePortName );
    }


    //-----------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
int explicit_scheme( int argc, char *argv[] )
{
    using namespace Foam;

#   include "setRootCase.H"

    TTaskMgrPtr aMgr = TTaskMgr::get_instance();

    //---------------------------------------------------------------------------
    // creation of the tasks dedicated to the scheme
    CTimeSourceTask::TPtr aTimeSource( new CTimeSourceTask() );
    aTimeSource->setTime( dimensionedScalar( 0.000 ), 0 );
    aTimeSource->setDeltaT( dimensionedScalar( 0.005 ) );
    aTimeSource->setEndTime( dimensionedScalar( 0.010 ) );
    aTimeSource->setWriteInterval( 1 );

    CSolverTaskBase::TPtr aNuclearSolver = TNuclearSolverTask::create( args, true );

    CSolverTaskBase::TPtr aTemperatureSolver = TTemperatureSolverTask::create( args, true );

    //---------------------------------------------------------------------------
    // Defintion of the calculation scheme topology 
    connect( aMgr, aTimeSource, aNuclearSolver );

    connect( aMgr, aNuclearSolver, "powerDensity", aTemperatureSolver ); 

    connect( aMgr, aTimeSource, aTemperatureSolver );

    connect( aMgr, aTemperatureSolver, "Tfuel", aNuclearSolver ); 

    connect( aMgr, aTemperatureSolver, "Tmod", aNuclearSolver ); 

    //---------------------------------------------------------------------------
    // Registration in the task manager
    aMgr->add_task( aTemperatureSolver );
    aMgr->add_task( aNuclearSolver );
    aMgr->add_task( aTimeSource );

    //---------------------------------------------------------------------------
    // Starting the scheme calculation
    aMgr->start();
    
    aMgr->wait();

    return 0;
}


//---------------------------------------------------------------------------
int implicit_scheme( int argc, char *argv[] )
{
    using namespace Foam;

#   include "setRootCase.H"

    TTaskMgrPtr aMgr = TTaskMgr::get_instance();

    //---------------------------------------------------------------------------
    // creation of the tasks dedicated to the scheme & registration in the task manager
    CORPredicateTask::TPtr aCombinedConvergencyCriteria( new CORPredicateTask() );
    aMgr->add_task( aCombinedConvergencyCriteria );

    CTimeSourceTask::TPtr aTimeSource( new CTimeSourceTask() );
    aTimeSource->setTime( dimensionedScalar( 0.000 ), 0 );
    aTimeSource->setDeltaT( dimensionedScalar( 0.005 ) );
    aTimeSource->setEndTime( dimensionedScalar( 0.010 ) );
    aTimeSource->setWriteInterval( 1 );
    aMgr->add_task( aTimeSource );

    CSolverTaskBase::TPtr aNuclearSolver = TNuclearSolverTask::create( args, true );
    aMgr->add_task( aNuclearSolver );

    CConvergencyCriteriaTask::TPtr aNuclearConvergencyCriteria( new CConvergencyCriteriaTask() );
    aNuclearConvergencyCriteria->setMaxResidual( 1.0E-7 );
    aNuclearConvergencyCriteria->setMaxIterationNumber( 100 );
    aMgr->add_task( aNuclearConvergencyCriteria );

    CSolverTaskBase::TPtr aTemperatureSolver = TTemperatureSolverTask::create( args, true );
    aMgr->add_task( aTemperatureSolver );

    CConvergencyCriteriaTask::TPtr aTemperatureConvergencyCriteria( new CConvergencyCriteriaTask() );
    aTemperatureConvergencyCriteria->setMaxResidual( 1.0E-7 );
    aTemperatureConvergencyCriteria->setMaxIterationNumber( 10 );
    aMgr->add_task( aTemperatureConvergencyCriteria );

    //---------------------------------------------------------------------------
    // Defintion of the calculation scheme topology 
    aMgr->connect( aCombinedConvergencyCriteria, "result", aTimeSource, "next" );

    connect( aMgr, aTimeSource, aNuclearSolver );

    connect( aMgr, aNuclearSolver, aNuclearConvergencyCriteria );

    connect( aMgr, aNuclearSolver, "powerDensity", aTemperatureSolver ); 

    connect( aMgr, aTimeSource, aTemperatureSolver );

    connect( aMgr, aTemperatureSolver, aTemperatureConvergencyCriteria );

    connect( aMgr, aTemperatureSolver, "Tfuel", aNuclearSolver ); 

    connect( aMgr, aTemperatureSolver, "Tmod", aNuclearSolver ); 

    connect( aMgr, aTemperatureConvergencyCriteria, aNuclearConvergencyCriteria, aCombinedConvergencyCriteria );

    aMgr->connect( aCombinedConvergencyCriteria, "result", aTimeSource, "next" );

    //---------------------------------------------------------------------------
    // Starting the scheme calculation
    aMgr->start();
    
    aMgr->wait();

    return 0;
}


//---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    return implicit_scheme( argc, argv );
}


//---------------------------------------------------------------------------
 
