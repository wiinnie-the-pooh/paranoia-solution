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
#include "parallel/dev/CConvergencyCriteriaTask.h"

#include <iostream>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //-----------------------------------------------------------------------
    CConvergencyCriteriaTask::CConvergencyCriteriaTask()
      : m_residual_i( "residual", true, *this )
      , m_stop_i( "stop", true, *this )

      , m_finished_o( "finished", false, *this )
      , m_result_o( "result", false, *this )

      , maxResidual( 0.001 )
      , maxIterationNumber( 100 )
      , iterationCounter( 0 )
    {}


    //-----------------------------------------------------------------------
    CConvergencyCriteriaTask::~CConvergencyCriteriaTask()
    {}
 

    //-----------------------------------------------------------------------
    void CConvergencyCriteriaTask::setMaxResidual( double the_residual )
    {
      this->maxResidual = the_residual;
    }


    //-----------------------------------------------------------------------
    double CConvergencyCriteriaTask::getMaxResidual() const
    {
      return this->maxResidual;
    }


    //-----------------------------------------------------------------------
    void CConvergencyCriteriaTask::setMaxIterationNumber( int the_number )
    {
      this->maxIterationNumber = the_number;
    }


    //-----------------------------------------------------------------------
    int CConvergencyCriteriaTask::getMaxIterationNumber() const
    {
      return this->maxIterationNumber;
    }


    //-----------------------------------------------------------------------
    void CConvergencyCriteriaTask::init()
    {
      MSG( "Start of CConvergencyCriteriaTask[ " << this << " ]\n" );
    }
    
    
    //-----------------------------------------------------------------------
    bool CConvergencyCriteriaTask::step()
    {
      bool stop_iterations = this->m_stop_i.retrieve();
      if ( ! stop_iterations )
      {
        bool satisfy_residual = this->m_residual_i.retrieve() < this->maxResidual;
        
        bool exit_iterations_limit = this->iterationCounter++ > this->maxIterationNumber;
        
        stop_iterations = satisfy_residual || exit_iterations_limit;
        
        if ( stop_iterations )
          this->iterationCounter = 0;

        MSG( "CConvergencyCriteriaTask::step[ " << this << " ]" 
	     << " | satisfy_residual = " << satisfy_residual
	     << " | exit_iterations_limit = " << exit_iterations_limit
	     << " | m_stop_i = " << this->m_stop_i()
	     << "\n" ); 
      }

      this->m_result_o.publish( stop_iterations );

      this->m_finished_o.publish( this->m_stop_i() );

      return ! this->m_finished_o();
    }


    //-----------------------------------------------------------------------
    void CConvergencyCriteriaTask::destroy()
    {
      MSG( "End of CConvergencyCriteriaTask[ " << this << " ]\n" );
    }
    

    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
