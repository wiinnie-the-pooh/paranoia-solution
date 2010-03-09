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
#ifndef toys_SolverExamples_h
#define toys_SolverExamples_h


//---------------------------------------------------------------------------
#include "parallel/toys/TRawDataTask.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    struct SleepSolver : TRawDataTask
    {
      SleepSolver( int theMSecs );

    protected:
      void sleep();

    private:
      int m_MSecs;
    };

  
    //---------------------------------------------------------------------------
    struct DataSource : SleepSolver
    {
      DataSource( int theMSecs );

    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Iter;
    };


    //---------------------------------------------------------------------------
    struct Copy : SleepSolver
    {
      Copy( int theMSecs );

    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct Palindrome : SleepSolver
    {
      Palindrome( int theMSecs );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct Concat : SleepSolver
    {
      Concat( int theMSecs );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct PlusX : SleepSolver
    {
      PlusX( int theMSecs, int theStart, int theDelta );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Start, m_Delta;
    };


    //---------------------------------------------------------------------------
    struct MultX : SleepSolver
    {
      MultX( int theMSecs, int theStart, int theMult );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Start, m_Mult;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
